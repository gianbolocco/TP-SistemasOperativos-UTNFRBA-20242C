#include "operaciones_cpu.h"

void procesar_peticion_cpu(int socket_cliente)
{
    // Mantengo conectada la CPU hasta que haya un error o un mensaje desconocido
    bool error = false;
    while (!error)
    {
        log_debug(logger_aux, "Esperando mensaje de CPU");
        t_package *paquete = recibir_paquete(socket_cliente);
        char *codigo = obtener_codigo_operacion(paquete->codigo_operacion);
        log_info(logger_aux, "Codigo de operacion recibido: %s", codigo);
        usleep(config_datos->retardo_respuesta * 1000);
        switch (paquete->codigo_operacion)
        {
        case OBTENER_CONTEXTO:
            obtener_contexto(paquete, socket_cliente);
            break;
        case ACTUALIZAR_CONTEXTO:
            actualizar_contexto(paquete, socket_cliente);
            break;
        case OBTENER_INSTRUCCION:
            obtener_instruccion(paquete, socket_cliente);
            break;
        case READ_MEM:
            read_mem(paquete, socket_cliente);
            break;
        case WRITE_MEM:
            write_mem(paquete, socket_cliente);
            break;
        default:
            error = true;
            break;
        }
        eliminar_paquete(paquete);
    }
}

void obtener_contexto(t_package *paquete, int socket_cliente)
{
    // recibo el pqaquete todo codificado
    t_list *lista = deserealizar_paquete(paquete);
    // ORDEN POR FAVOR!!!
    int pid = *((int *)list_get(lista, 0)); // Deserializar osea voy leyendo de a partes lo que me mando
    int tid = *((int *)list_get(lista, 1));
    t_pcb_memoria *pcb = buscar_pcb_tid(pid, tid);

    if (pcb == NULL)
    {
        list_destroy_and_destroy_elements(lista, free);
        log_error(logger, "PCB no encontrada");
        t_package *paquete_respuesta = crear_paquete(PID_NOT_FOUND, buffer_vacio());
        enviar_paquete(paquete_respuesta, socket_cliente);
        eliminar_paquete(paquete_respuesta);
        return;
    }

    t_package *paquete_respuesta = crear_paquete(PCONTEXTO, buffer_vacio());
    // codigo para cpu
    agregar_a_paquete(paquete_respuesta, &(pcb->base), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->limite), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->AX), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->BX), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->CX), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->DX), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->EX), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->FX), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->GX), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->HX), sizeof(uint32_t));
    agregar_a_paquete(paquete_respuesta, &(pcb->PC), sizeof(uint32_t));

    log_debug(logger, "Contexto del PID %d, TID %d enviado: PC %u, AX %u, BX %u, CX %u, DX %u, EX %u, FX %u, GX %u, HX %u, base %u, limite %u", pid, tid, pcb->PC, pcb->AX, pcb->BX, pcb->CX, pcb->DX, pcb->EX, pcb->FX, pcb->GX, pcb->HX, pcb->base, pcb->limite);

    log_info(logger, "## Contexto Solicitados - (PID:TID) - (%d:%d)", pid, tid);

    enviar_paquete(paquete_respuesta, socket_cliente);

    eliminar_paquete(paquete_respuesta);
    list_destroy_and_destroy_elements(lista, free);
}

void actualizar_contexto(t_package *paquete, int socket_cliente)
{
    // recibo el pqaquete todo codificado
    t_list *lista = deserealizar_paquete(paquete);
    // ORDEN POR FAVOR!!!
    int pid = *((int *)list_get(lista, 0)); // Deserializar osea voy leyendo de a partes lo que me mando
    int tid = *((int *)list_get(lista, 1));
    uint32_t pc = *((uint32_t *)list_get(lista, 2));
    uint32_t AX = *((uint32_t *)list_get(lista, 3));
    uint32_t BX = *((uint32_t *)list_get(lista, 4));
    uint32_t CX = *((uint32_t *)list_get(lista, 5));
    uint32_t DX = *((uint32_t *)list_get(lista, 6));
    uint32_t EX = *((uint32_t *)list_get(lista, 7));
    uint32_t FX = *((uint32_t *)list_get(lista, 8));
    uint32_t GX = *((uint32_t *)list_get(lista, 9));
    uint32_t HX = *((uint32_t *)list_get(lista, 10));
    uint32_t base = *((uint32_t *)list_get(lista, 11));
    uint32_t limite = *((uint32_t *)list_get(lista, 12));

    t_pcb_memoria *pcb = buscar_pcb_tid(pid, tid);
    if (pcb == NULL)
    {
        list_destroy_and_destroy_elements(lista, free);
        log_error(logger, "PCB no encontrada");
        t_package *paquete_respuesta = crear_paquete(PID_NOT_FOUND, buffer_vacio());
        enviar_paquete(paquete_respuesta, socket_cliente);
        eliminar_paquete(paquete_respuesta);
        return;
    }

    // actualizamos los registros
    pcb->PC = pc;
    pcb->AX = AX;
    pcb->BX = BX;
    pcb->CX = CX;
    pcb->DX = DX;
    pcb->EX = EX;
    pcb->FX = FX;
    pcb->GX = GX;
    pcb->HX = HX;
    pcb->base = base;
    pcb->limite = limite;

    log_info(logger, "## Contexto Actualizado - (PID:TID) - (%d:%d)", pid, tid);

    t_package *paquete_respuesta = crear_paquete(SUCCESS, buffer_vacio());
    enviar_paquete(paquete_respuesta, socket_cliente);
    eliminar_paquete(paquete_respuesta);
    list_destroy_and_destroy_elements(lista, free);
}

void obtener_instruccion(t_package *paquete, int socket_cliente)
{
    t_list *lista = deserealizar_paquete(paquete);
    int pid = *((int *)list_get(lista, 0));
    int tid = *((int *)list_get(lista, 1));
    int pc = *((int *)list_get(lista, 2)); // es un indice

    t_pcb_memoria *pcb = buscar_pcb_tid(pid, tid);
    if (pcb == NULL)
    {
        list_destroy_and_destroy_elements(lista, free);
        log_error(logger, "PCB no encontrada pid: %d - tid: %d", pid, tid);
        t_package *paquete_respuesta = crear_paquete(PID_NOT_FOUND, buffer_vacio());
        enviar_paquete(paquete_respuesta, socket_cliente);
        eliminar_paquete(paquete_respuesta);
        return;
    }

    if (pc >= list_size(pcb->instrucciones))
    {
        list_destroy_and_destroy_elements(lista, free);
        log_error(logger_aux, "No existe la instruccion a la que se desea acceder pid: %d - tid: %d - pc: %d", pid, tid, pc);
        t_package *paquete_respuesta = crear_paquete(INSTRUCCION_NOT_FOUND, buffer_vacio());
        enviar_paquete(paquete_respuesta, socket_cliente);
        eliminar_paquete(paquete_respuesta);
        return;
    }
    char *instruccion = string_duplicate(strtok((char *)list_get(pcb->instrucciones, pc), "\n"));

    t_package *paquete_respuesta = crear_paquete(INSTRUCCION, buffer_vacio());

    int32_t tamanio = string_length(instruccion) + 1;

    agregar_a_paquete(paquete_respuesta, instruccion, tamanio);

    enviar_paquete(paquete_respuesta, socket_cliente);

    log_info(logger, "## Obtener instrucción - (PID:TID) - (%d:%d) - Instrucción: %s", pid, tid, instruccion);

    eliminar_paquete(paquete_respuesta);
    list_destroy_and_destroy_elements(lista, free);
    free(instruccion);
}

void read_mem(t_package *paquete, int socket_cliente)
{
    t_list *lista = deserealizar_paquete(paquete);
    int pid = *((int *)list_get(lista, 0));
    int tid = *((int *)list_get(lista, 1));
    int df = *((int *)list_get(lista, 2));
    void *valor_leido = malloc(sizeof(uint32_t));

    if ((df + sizeof(uint32_t)) >= config_datos->tamanio_memoria)
    {
        list_destroy_and_destroy_elements(lista, free);
        log_error(logger, "Dirección física fuera de rango pid: %d - tid: %d - df: %d", pid, tid, df);
        t_package *paquete_respuesta = crear_paquete(SEGFAULT, buffer_vacio());
        enviar_paquete(paquete_respuesta, socket_cliente);
        eliminar_paquete(paquete_respuesta);
        return;
    }

    pthread_mutex_lock(&memory_space_mutex);
    memcpy(valor_leido, memoria + df, sizeof(uint32_t)); // Acceder a memory_space. Osea las funciones de lectura y escritura acceden directamente a memory_space inicializado en start_memory().
    pthread_mutex_unlock(&memory_space_mutex);
    // puse mi semaforo en la region critica porque CPU es la que me la da

    uint32_t valor = *((uint32_t *)valor_leido);

    log_info(logger_aux, "Valor leído desde la dirección lógica %d: %d", df, valor);

    log_info(logger, "## Lectura - (PID:TID) -(%d:%d) - Dir. Física: %d - Tamaño: %d", pid, tid, df, 4);
    // le voy a mandar la respuesta que quiere tom
    t_package *paquete_respuesta = crear_paquete(INFO, buffer_vacio());
    // codigo para cpu

    agregar_a_paquete(paquete_respuesta, valor_leido, sizeof(uint32_t));

    enviar_paquete(paquete_respuesta, socket_cliente);

    eliminar_paquete(paquete_respuesta);
    list_destroy_and_destroy_elements(lista, free);
}

void write_mem(t_package *paquete, int socket_cliente)
{
    t_list *lista = deserealizar_paquete(paquete);
    int pid = *((int *)list_get(lista, 0));
    int tid = *((int *)list_get(lista, 1));
    int df = *((int *)list_get(lista, 2));
    uint32_t registro = *((uint32_t *)list_get(lista, 3));

    if ((df + sizeof(uint32_t)) >= config_datos->tamanio_memoria)
    {
        list_destroy_and_destroy_elements(lista, free);
        log_error(logger, "Dirección física fuera de rango");
        t_package *paquete_respuesta = crear_paquete(SEGFAULT, buffer_vacio());
        enviar_paquete(paquete_respuesta, socket_cliente);
        eliminar_paquete(paquete_respuesta);
        return;
    }

    pthread_mutex_lock(&memory_space_mutex);
    memcpy(memoria + df, &registro, sizeof(uint32_t)); // escribiendo directamente en una direccion de memoria. Memcopy recibe una direccion base + 15 bytes que se mueve a la drecha y escribe todos los datos de registro que le paso por sizeof
    pthread_mutex_unlock(&memory_space_mutex);
    // aca limito mi region critic

    log_info(logger_aux, "Valor escrito en la dirección lógica %d: %d", df, registro);
    // TODO: ARREGLAR TID Y TAMANIO NO LOS RECIBO DE CPU
    log_info(logger, "## Escritura - (PID:TID) -(%d:%d) - Dir. Física: %d - Tamaño: %d", pid, tid, df, 4);

    t_package *paquete_respuesta = crear_paquete(SUCCESS, buffer_vacio());
    enviar_paquete(paquete_respuesta, socket_cliente);
    eliminar_paquete(paquete_respuesta);
    list_destroy_and_destroy_elements(lista, free);
}