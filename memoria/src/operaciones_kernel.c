#include "operaciones_kernel.h"

void procesar_peticion_kernel(int socket_cliente)
{
    log_debug(logger_aux, "Esperando mensaje de kernel");
    t_package *paquete = recibir_paquete(socket_cliente);
    char *codigo = obtener_codigo_operacion(paquete->codigo_operacion);
    log_info(logger_aux, "MEMORIA - KERNEL: Codigo de operacion recibido: %s", codigo);
    switch (paquete->codigo_operacion)
    {
    case CREAR_PROCESO:
        creacion_de_proceso(paquete, socket_cliente);
        break;
    case FIN_PROCESO:
        finalizacion_de_proceso(paquete, socket_cliente);
        break;
    case CREAR_HILO:
        creacion_de_hilo(paquete, socket_cliente);
        break;
    case FIN_HILO:
        finalizacion_de_hilo(paquete, socket_cliente);
        break;
    case MEMORY_DUMP:
        memory_dump(paquete, socket_cliente);
        break;
    default:
        break;
    }
    eliminar_paquete(paquete);
}

void creacion_de_proceso(t_package *paquete, int socket_cliente)
{
    // recibo el pqaquete todo codificado
    t_list *lista = deserealizar_paquete(paquete);
    // ORDEN POR FAVOR!!!
    int pid = *((int *)list_get(lista, 0)); // Deserializar osea voy leyendo de a partes lo que me mando
    int tamanio = *((int *)list_get(lista, 1));
    char *path = (char *)list_get(lista, 2);

    log_info(logger, "Creando proceso %d", pid);
    t_pcb_memoria *pcbProceso = malloc(sizeof(t_pcb_memoria)); // inicialice, cree una variable de tipo pcb de utlis.h
    pcbProceso->pid = pid;
    pcbProceso->tid = 0;
    pcbProceso->AX = 0; // por definicion de catedra
    pcbProceso->BX = 0;
    pcbProceso->CX = 0;
    pcbProceso->DX = 0;
    pcbProceso->EX = 0;
    pcbProceso->FX = 0;
    pcbProceso->GX = 0;
    pcbProceso->HX = 0;
    pcbProceso->PC = 0;
    pcbProceso->tamanio = tamanio;

    // 1) veo si mi prceso entra en memoria sino le digo no hay espacio a kernel
    op_code resultado = entra_en_memoria(tamanio);
    if (resultado != SUCCESS)
    {
        if (resultado == MAX_SIZE_ERROR)
        {
            log_error(logger, "El espacio solicitado es mayor al maximo permitido en memoria para el proceso %d - tid: %d - tamanio: %d", pcbProceso->pid, pcbProceso->tid, tamanio);
        }
        else if (resultado == SIZE_ERROR)
        {
            log_error(logger, "No hay espacio suficiente en memoria para el proceso %d - tid: %d", pcbProceso->pid, pcbProceso->tid);
        }
        list_destroy_and_destroy_elements(lista, free);
        t_package *respuesta = crear_paquete(resultado, buffer_vacio());
        enviar_paquete(respuesta, socket_cliente);
        eliminar_paquete(respuesta);
        free(pcbProceso); // borramos memoria
        return;           // lo descarto de una parta no tener un codigo anidado
    }

    // 2) compacto si hacia falta, lo compato y todo bien
    /* if (string_equals_ignore_case(config_datos->esquema, "DINAMICAS") && requiere_compactar(tamanio))
    {
        compactar();
    } */

    // asignar particion devuelve un t_particion y lo almaceno en particion
    t_particion *particion = asignar_particion(tamanio);
    if (particion == NULL)
    {
        log_error(logger, "No se pudo asignar particion para el proceso %d - tid: %d", pcbProceso->pid, pcbProceso->tid);
        list_destroy_and_destroy_elements(lista, free);
        t_package *respuesta = crear_paquete(SIZE_ERROR, buffer_vacio());
        enviar_paquete(respuesta, socket_cliente);
        eliminar_paquete(respuesta);
        free(pcbProceso); // borramos memoria
        return;
    }

    // para mi proceso, la base desde donte tiene acceso a memoria es la particion
    pcbProceso->base = particion->inicio;
    pcbProceso->limite = particion->fin;

    // fread(&rauxS, sizeof(struct codProd), 1, S);
    // leo las instrucciones
    char *finalPath = string_new();
    string_append(&finalPath, config_datos->path_instrucciones);
    string_append(&finalPath, "/");
    string_append(&finalPath, path);
    log_debug(logger, "Leyendo instrucciones desde: %s", finalPath);
    FILE *file_instrucciones = fopen(finalPath, "rt");
    if (file_instrucciones == NULL)
    {
        list_destroy_and_destroy_elements(lista, free);
        free(finalPath);
        log_error(logger, "No se pudo leer el archivo :(");
        t_package *respuesta = crear_paquete(PATH_ERROR, buffer_vacio());
        enviar_paquete(respuesta, socket_cliente);
        eliminar_paquete(respuesta);
        free(pcbProceso); // borramos memoria
        return;
    }

    t_list *lista_instrucciones = list_create();
    size_t len = 0;    // tamanio de la linea leida - USADAS PARA GETLINE
    size_t read;       // cant de caracteres leido - USADAS PARA GETLINE
    char *line = NULL; // linea leida
    while ((read = getline(&line, &len, file_instrucciones)) != -1)
    {
        char *t = strtok(line, "\n");
        char *y = strtok(t, "\r");
        // cada linea es una instruccion
        char *instruccion = string_duplicate(y);
        list_add(lista_instrucciones, instruccion);
    }
    free(line);
    fclose(file_instrucciones);

    pcbProceso->instrucciones = lista_instrucciones;

    list_add(tablaPCBs, pcbProceso);
    log_info(logger, "## Proceso Creado - PID: %d - Tamaño: %d", pid, tamanio);

    t_package *respuesta = crear_paquete(SUCCESS, buffer_vacio());
    enviar_paquete(respuesta, socket_cliente);
    eliminar_paquete(respuesta);
    list_destroy_and_destroy_elements(lista, free);
    free(finalPath);
}

void finalizacion_de_proceso(t_package *paquete, int socket_cliente)
{
    // recibo el paquete todo codificado
    t_list *lista = deserealizar_paquete(paquete);
    // ORDEN POR FAVOR!!!
    int pid = *((int *)list_get(lista, 0)); // Deserializar osea voy leyendo de a partes lo que me mando
    list_destroy_and_destroy_elements(lista, free);
    t_list *lista_de_hilos_de_proceso = buscar_pcb(pid);
    if (lista_de_hilos_de_proceso == NULL)
    {
        log_error(logger, "Proceso no encontrada");
        t_package *paquete_respuesta = crear_paquete(PID_NOT_FOUND, buffer_vacio());
        enviar_paquete(paquete_respuesta, socket_cliente);
        eliminar_paquete(paquete_respuesta);
        return;
    }

    uint32_t tamanio = 0;
    for (int i = 0; i < list_size(lista_de_hilos_de_proceso); i++)
    {
        t_pcb_memoria *pcb = list_get(lista_de_hilos_de_proceso, i);
        if (pcb->tid == 0)
        {
            tamanio = pcb->tamanio;
            liberar_proceso(pcb);
        }
        liberar_hilo(pcb);
    }
    list_destroy(lista_de_hilos_de_proceso);

    log_info(logger, "## Proceso Destruido - PID: %d - Tamaño: %d", pid, tamanio);

    t_package *paquete_respuesta = crear_paquete(SUCCESS, buffer_vacio());
    enviar_paquete(paquete_respuesta, socket_cliente);
    eliminar_paquete(paquete_respuesta);
}

void creacion_de_hilo(t_package *paquete, int socket_cliente)
{
    // PENDIENTE
    t_list *lista = deserealizar_paquete(paquete);
    int pid = *((int *)list_get(lista, 0)); // Deserializar osea voy leyendo de a partes lo que me mando
    int tid = *((int *)list_get(lista, 1)); // Deserializar osea voy leyendo de a partes lo que me mando
    char *path = (char *)list_get(lista, 2);

    // Busco al proceso padre
    t_pcb_memoria *pcb_hilo_main = buscar_pcb_tid(pid, 0);

    log_info(logger, "Creando hilo pid: %d tid: %d", pid, tid);
    t_pcb_memoria *pcbHilo = malloc(sizeof(t_pcb_memoria)); // inmcialice, cree una variable de tipo pcb de utlis.h
    pcbHilo->pid = pid;
    pcbHilo->tid = tid;
    pcbHilo->AX = 0; // por definicion de catedra
    pcbHilo->BX = 0;
    pcbHilo->CX = 0;
    pcbHilo->DX = 0;
    pcbHilo->EX = 0;
    pcbHilo->FX = 0;
    pcbHilo->GX = 0;
    pcbHilo->HX = 0;
    pcbHilo->PC = 0;
    pcbHilo->tamanio = pcb_hilo_main->tamanio;
    pcbHilo->base = pcb_hilo_main->base;
    pcbHilo->limite = pcb_hilo_main->limite;

    // leo las instrucciones
    char *finalPath = string_new();
    string_append(&finalPath, config_datos->path_instrucciones);
    string_append(&finalPath, "/");
    string_append(&finalPath, path);
    log_debug(logger, "Leyendo instrucciones desde: %s", finalPath);
    FILE *file_instrucciones = fopen(finalPath, "rt");
    if (file_instrucciones == NULL)
    {
        log_error(logger, "No se pudo leer el archivo :(");
        t_package *respuesta = crear_paquete(PATH_ERROR, buffer_vacio());
        enviar_paquete(respuesta, socket_cliente);
        free(pcbHilo); // borramos memoria
        return;
    }

    t_list *lista_instrucciones = list_create();
    size_t len = 0;    // tamanio de la linea leida - USADAS PARA GETLINE
    size_t read;       // cant de caracteres leido - USADAS PARA GETLINE
    char *line = NULL; // linea leida
    while ((read = getline(&line, &len, file_instrucciones)) != -1)
    {
        // cada linea es una isntruccion
        char *t = strtok(line, "\n");
        char *y = strtok(t, "\r");
        // cada linea es una instruccion
        char *instruccion = string_duplicate(y);
        list_add(lista_instrucciones, instruccion);
    }
    free(line);
    fclose(file_instrucciones);

    pcbHilo->instrucciones = lista_instrucciones;

    list_add(tablaPCBs, pcbHilo);
    log_info(logger, "## Hilo Creado - (PID:TID) - (%d:%d)", pid, tid);

    t_package *paquete_respuesta = crear_paquete(SUCCESS, buffer_vacio());
    enviar_paquete(paquete_respuesta, socket_cliente);
    eliminar_paquete(paquete_respuesta);
    free(finalPath);
    list_destroy_and_destroy_elements(lista, free);
}

void finalizacion_de_hilo(t_package *paquete, int socket_cliente)
{
    t_list *lista = deserealizar_paquete(paquete);
    // ORDEN POR FAVOR!!!
    int pid = *((int *)list_get(lista, 0)); // Deserializar osea voy leyendo de a partes lo que me mando
    int tid = *((int *)list_get(lista, 1)); // Deserializar osea voy leyendo de a partes lo que me mando

    t_pcb_memoria *pcb = buscar_pcb_tid(pid, tid);
    if (pcb == NULL)
    {
        list_destroy_and_destroy_elements(lista, free);
        log_error(logger, "PCB no encontrada");
        t_package *paquete_respuesta = crear_paquete(TID_NOT_FOUND, buffer_vacio());
        enviar_paquete(paquete_respuesta, socket_cliente);
        eliminar_paquete(paquete_respuesta);
        return;
    }

    liberar_hilo(pcb);

    log_info(logger, "## Hilo Destruido - (PID:TID) - (%d:%d)", pid, tid);

    t_package *paquete_respuesta = crear_paquete(SUCCESS, buffer_vacio());
    enviar_paquete(paquete_respuesta, socket_cliente);
    eliminar_paquete(paquete_respuesta);
    list_destroy_and_destroy_elements(lista, free);
}

void memory_dump(t_package *paquete, int socket_cliente)
{
    // VER LO QUE TIENE UN TROZO DE LA MEMORIA EN ESE MOMENTO
    t_list *lista = deserealizar_paquete(paquete);
    int pid = *((int *)list_get(lista, 0));
    int tid = *((int *)list_get(lista, 1));
    t_pcb_memoria *pcb = buscar_pcb_tid(pid, tid);
    //char *dump = mem_hexstring(memoria + pcb->base, pcb->limite);
    char *timestamp = temporal_get_string_time("%H:%M:%S:%MS");
    char *nombre = string_from_format("%d-%d-%s.dmp", pid, tid, timestamp);
    // vamos a conectaros al fs, mandar el paquete y desconectarnos  // es un socket el conex fs
    int conex_fs = create_connection(config_datos->ip_filesystem, config_datos->puerto_filesystem, logger);

    if (conex_fs == -1)
    {
        log_error(logger, "Error conexion con filesystrem");
        return;
    }

    // Handshake:
    // quien soy //A QUIEN ME CONECTO
    hs_client_to_module_valid(conex_fs, HSMEMORIA, HSFS, logger);

    t_package *paquete_respuesta = crear_paquete(MEMORY_DUMP, buffer_vacio());
    uint32_t tam_proceso = (pcb->limite - pcb->base) + 1;

    void *dump = malloc(tam_proceso);
    memcpy(dump, memoria + pcb->base, tam_proceso);

    agregar_a_paquete(paquete_respuesta, &tam_proceso, sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, nombre, string_length(nombre) + 1);
    //agregar_a_paquete(paquete_respuesta, dump, string_length(dump) + 1);
    agregar_a_paquete(paquete_respuesta, dump, tam_proceso);
    enviar_paquete(paquete_respuesta, conex_fs);

    log_info(logger, "## Memory Dump solicitado - (PID:TID) - (%d:%d)", pid, tid);
    t_package *paquete_resultado = recibir_paquete(conex_fs);
    if (paquete_resultado->codigo_operacion != SUCCESS)
    {
        log_error(logger, "Error al escribir en filesystem %s", obtener_codigo_operacion(paquete_resultado->codigo_operacion));
    } else {
        log_debug(logger_aux, "Se escribio bien en filesystem");
    }

    t_package *paquete_respuesta_kernel = crear_paquete(paquete_resultado->codigo_operacion, buffer_vacio());
    enviar_paquete(paquete_respuesta_kernel, socket_cliente);

    eliminar_paquete(paquete_respuesta);

    eliminar_paquete(paquete_resultado);
    eliminar_paquete(paquete_respuesta_kernel);
    socket_destroy(conex_fs);
    list_destroy_and_destroy_elements(lista, free);
    free(dump);
    free(timestamp);
    free(nombre);
}
