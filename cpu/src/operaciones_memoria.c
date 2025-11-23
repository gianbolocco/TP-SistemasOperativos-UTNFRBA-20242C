#include "operaciones_memoria.h"

void iniciar_conexion_memoria()
{
    socket_memoria = create_connection(config_cpu->ip_memoria, config_cpu->puerto_memoria, logger_aux);

    if (socket_memoria == -1)
    {
        log_error(logger_aux, "Error al conectar con la memoria");
        return;
    }

    hs_client_to_module_valid(socket_memoria, HSCPU, HSMEMORIA, logger_aux);
}

void finalizar_conexion_memoria()
{
    if (socket_memoria != -1)
    {
        socket_destroy(socket_memoria);
    }
}

void obtener_contexto()
{
    log_info(logger, "## TID: %d - Solicito Contexto Ejecucion", contexto->tid);
    t_package *request = crear_paquete(OBTENER_CONTEXTO, buffer_vacio());
    agregar_a_paquete(request, &contexto->pid, sizeof(uint32_t));
    agregar_a_paquete(request, &contexto->tid, sizeof(uint32_t));
    enviar_paquete(request, socket_memoria);
    eliminar_paquete(request);

    t_package *response = recibir_paquete(socket_memoria);
    if (response->codigo_operacion != PCONTEXTO)
    {
        log_error(logger_aux, "El codigo de operacion es invalido al obtener el contexto");
        flag_fin_programa = true;
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        eliminar_paquete(response);
        return;
    }
    t_list *datos = deserealizar_paquete(response);
    contexto->base = *((uint32_t *)list_get(datos, 0));
    contexto->limite = *((uint32_t *)list_get(datos, 1));
    contexto->ax = *((uint32_t *)list_get(datos, 2));
    contexto->bx = *((uint32_t *)list_get(datos, 3));
    contexto->cx = *((uint32_t *)list_get(datos, 4));
    contexto->dx = *((uint32_t *)list_get(datos, 5));
    contexto->ex = *((uint32_t *)list_get(datos, 6));
    contexto->fx = *((uint32_t *)list_get(datos, 7));
    contexto->gx = *((uint32_t *)list_get(datos, 8));
    contexto->hx = *((uint32_t *)list_get(datos, 9));
    contexto->pc = *((uint32_t *)list_get(datos, 10));
    eliminar_paquete(response);
    list_destroy_and_destroy_elements(datos, free);
    log_debug(logger_aux, "Se obtuvo el contexto del hilo %u del programa %u: base %u, limite %u, ax %u, bx %u, cx %u, dx %u, ex %u, fx %u, gx %u, hx %u, pc %u", contexto->tid, contexto->pid, contexto->base, contexto->limite, contexto->ax, contexto->bx, contexto->cx, contexto->dx, contexto->ex, contexto->fx, contexto->gx, contexto->hx, contexto->pc);
}

void obtener_intruccion()
{
    log_info(logger_aux, "Solicitando instruccion al hilo %d del programa %d", contexto->tid, contexto->pid);
    t_package *request = crear_paquete(OBTENER_INSTRUCCION, buffer_vacio());
    agregar_a_paquete(request, &contexto->pid, sizeof(uint32_t));
    agregar_a_paquete(request, &contexto->tid, sizeof(uint32_t));
    agregar_a_paquete(request, &contexto->pc, sizeof(uint32_t));
    enviar_paquete(request, socket_memoria);
    eliminar_paquete(request);

    t_package *response = recibir_paquete(socket_memoria);
    if (response->codigo_operacion != INSTRUCCION)
    {
        switch (response->codigo_operacion)
        {
        case PID_NOT_FOUND:
            log_error(logger_aux, "El (PID:TID) (%d:%d) no fue encontrado al obtener la instruccion", contexto->pid, contexto->tid);
            flag_desalojo = true;
            motivo_desalojo = SEGFAULT;
            break;
        case INSTRUCCION_NOT_FOUND:
            log_error(logger_aux, "La instruccion %d no fue encontrada al obtener la instruccion", contexto->pc);
            flag_desalojo = true;
            motivo_desalojo = SEGFAULT;
            break;
        default:
            log_error(logger_aux, "El codigo de operacion es invalido al obtener la instruccion");
            flag_fin_programa = true;
            flag_desalojo = true;
            flag_interrupt = true;
            motivo_desalojo = SEGFAULT;
            break;
        }
        eliminar_paquete(response);
        return;
    }
    t_list *datos = deserealizar_paquete(response);
    char *instruccion = (char *)list_get(datos, 0);
    contexto->instruccion = string_duplicate(instruccion);
    list_destroy(datos);
    eliminar_paquete(response);
    log_debug(logger_aux, "Se obtuvo la instruccion %s del hilo %d del programa %d", instruccion, contexto->tid, contexto->pid);
    free(instruccion);
}

void actualizar_contexto()
{
    log_info(logger, "## TID: %d - Actualizo Contexto Ejecucion", contexto->tid);
    log_debug(logger_aux, "Se almacenara el contexto del hilo %u del programa %u: base %u, limite %u, ax %u, bx %u, cx %u, dx %u, ex %u, fx %u, gx %u, hx %u, pc %u", contexto->tid, contexto->pid, contexto->base, contexto->limite, contexto->ax, contexto->bx, contexto->cx, contexto->dx, contexto->ex, contexto->fx, contexto->gx, contexto->hx, contexto->pc);
    t_package *request = crear_paquete(ACTUALIZAR_CONTEXTO, buffer_vacio());
    agregar_a_paquete(request, &(contexto->pid), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->tid), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->pc), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->ax), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->bx), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->cx), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->dx), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->ex), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->fx), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->gx), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->hx), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->base), sizeof(uint32_t));
    agregar_a_paquete(request, &(contexto->limite), sizeof(uint32_t));
    enviar_paquete(request, socket_memoria);
    eliminar_paquete(request);

    t_package *response = recibir_paquete(socket_memoria);
    if (response->codigo_operacion != SUCCESS)
    {
        log_error(logger_aux, "El codigo de operacion es invalido al actualizar el contexto");
        flag_fin_programa = true;
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        eliminar_paquete(response);
        return;
    }
    eliminar_paquete(response);
}

void read_mem()
{
    log_info(logger, "## TID: %d - Accion: LEER - Direccion Fisica: %d", contexto->tid, contexto->direccion_fisica);
    t_package *request = crear_paquete(READ_MEM, buffer_vacio());
    agregar_a_paquete(request, &contexto->pid, sizeof(uint32_t));
    agregar_a_paquete(request, &contexto->tid, sizeof(uint32_t));
    agregar_a_paquete(request, &contexto->direccion_fisica, sizeof(uint32_t));
    enviar_paquete(request, socket_memoria);
    eliminar_paquete(request);

    t_package *response = recibir_paquete(socket_memoria);
    if (response->codigo_operacion != INFO)
    {
        if (response->codigo_operacion == SEGFAULT)
        {
            log_error(logger_aux, "Segmentation Fault al leer la memoria");
            flag_desalojo = true;
            flag_interrupt = true;
            motivo_desalojo = SEGFAULT;
            eliminar_paquete(response);
            return;
        }
        log_error(logger_aux, "El codigo de operacion es invalido al leer la memoria");
        flag_fin_programa = true;
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        eliminar_paquete(response);
        return;
    }
    t_list *datos = deserealizar_paquete(response);
    uint32_t registro_valor = *((uint32_t *)list_get(datos, 0));
    set_registry(contexto->instruccion_parametros[0], registro_valor);
    eliminar_paquete(response);
    list_destroy_and_destroy_elements(datos, free);
}

void write_mem()
{
    log_info(logger, "## TID: %d - Accion: ESCRIBIR - Direccion Fisica: %d", contexto->tid, contexto->direccion_fisica);
    t_package *request = crear_paquete(WRITE_MEM, buffer_vacio());
    agregar_a_paquete(request, &contexto->pid, sizeof(uint32_t));
    agregar_a_paquete(request, &contexto->tid, sizeof(uint32_t));
    agregar_a_paquete(request, &contexto->direccion_fisica, sizeof(uint32_t));
    uint32_t valor = get_registry(contexto->instruccion_parametros[1]);
    agregar_a_paquete(request, &valor, sizeof(uint32_t));
    enviar_paquete(request, socket_memoria);
    eliminar_paquete(request);

    t_package *response = recibir_paquete(socket_memoria);
    if (response->codigo_operacion != SUCCESS)
    {
        if (response->codigo_operacion == SEGFAULT)
        {
            log_error(logger_aux, "Segmentation Fault al escribir en la memoria");
            flag_desalojo = true;
            flag_interrupt = true;
            motivo_desalojo = SEGFAULT;
            eliminar_paquete(response);
            return;
        }
        log_error(logger_aux, "El codigo de operacion es invalido al escribir en la memoria");
        flag_fin_programa = true;
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        eliminar_paquete(response);
    }
    eliminar_paquete(response);
}
