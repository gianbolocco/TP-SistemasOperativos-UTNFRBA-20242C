#include "operaciones_kernel.h"

void recibir_hilo_a_ejecutar(int socket_cliente)
{
    log_info(logger_aux, "Recibiendo hilo a ejecutar del kernel.");
    t_package *paquete = recibir_paquete(socket_cliente);
    t_list *datos = deserealizar_paquete(paquete);

    if (paquete->codigo_operacion != EJECUCION_HILO)
    {
        log_error(logger_aux, "El codigo de operacion es invalido.");
        flag_fin_programa = true;
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        eliminar_paquete(paquete);
        return;
    }
    uint32_t pid = *((uint32_t *)list_get(datos, 0));
    uint32_t tid = *((uint32_t *)list_get(datos, 1));

    contexto->pid = pid;
    contexto->tid = tid;

    eliminar_paquete(paquete);
    list_destroy_and_destroy_elements(datos, free);
}

void enviar_motivo_de_desalojo(int socket_cliente)
{
    char *motivo_desalojo_str = motivo_desalojo_to_string(motivo_desalojo);
    log_info(logger_aux, "Enviando motivo de desalojo al kernel %s.", motivo_desalojo_str);
    char *p0 = NULL, *p1 = NULL, *p2 = NULL;
    t_package *paquete = crear_paquete(MOTIVO_DESALOJO, buffer_vacio());
    agregar_a_paquete(paquete, &contexto->pid, sizeof(uint32_t));
    agregar_a_paquete(paquete, &contexto->tid, sizeof(uint32_t));
    agregar_a_paquete(paquete, &motivo_desalojo, sizeof(t_motivo_desalojo));

    switch (motivo_desalojo)
    {
    case M_IO:
        p0 = string_duplicate(contexto->instruccion_parametros[0]);
        agregar_a_paquete(paquete, p0, string_length(p0) + 1);
        break;
    case M_PROCESS_CREATE:
        p0 = string_duplicate(contexto->instruccion_parametros[0]);
        p1 = string_duplicate(contexto->instruccion_parametros[1]);
        p2 = string_duplicate(contexto->instruccion_parametros[2]);
        agregar_a_paquete(paquete, p0, string_length(p0) + 1);
        agregar_a_paquete(paquete, p1, string_length(p1) + 1);
        agregar_a_paquete(paquete, p2, string_length(p2) + 1);
        break;
    case M_THREAD_CREATE:
        p0 = string_duplicate(contexto->instruccion_parametros[0]);
        p1 = string_duplicate(contexto->instruccion_parametros[1]);
        agregar_a_paquete(paquete, p0, string_length(p0) + 1);
        agregar_a_paquete(paquete, p1, string_length(p1) + 1);
        break;
    case M_THREAD_JOIN:
        p0 = string_duplicate(contexto->instruccion_parametros[0]);
        agregar_a_paquete(paquete, p0, string_length(p0) + 1);
        break;
    case M_THREAD_CANCEL:
        p0 = string_duplicate(contexto->instruccion_parametros[0]);
        agregar_a_paquete(paquete, p0, string_length(p0) + 1);
        break;
    case M_MUTEX_CREATE:
        p0 = string_duplicate(contexto->instruccion_parametros[0]);
        agregar_a_paquete(paquete, p0, string_length(p0) + 1);
        break;
    case M_MUTEX_LOCK:
        p0 = string_duplicate(contexto->instruccion_parametros[0]);
        agregar_a_paquete(paquete, p0, string_length(p0) + 1);
        break;
    case M_MUTEX_UNLOCK:
        p0 = string_duplicate(contexto->instruccion_parametros[0]);
        agregar_a_paquete(paquete, p0, string_length(p0) + 1);
        break;
    default:
        break;
    }
    
    enviar_paquete(paquete, socket_cliente);
    eliminar_paquete(paquete);
    if (p0 != NULL)
        free(p0);
    if (p1 != NULL)
        free(p1);
    if (p2 != NULL)
        free(p2);
}