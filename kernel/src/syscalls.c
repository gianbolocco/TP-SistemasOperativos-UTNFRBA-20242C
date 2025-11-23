#include "syscalls.h"

void interrupcion()
{
    t_tcb *hilo = hilo_en_exec;
    desencolar_estado_actual(hilo);
    encolar_en_ready(hilo);
    log_info(logger_aux, "## (<PID>: %d - <TID>: %d) finalizó por interrupción y pasa a READY", hilo->pid, hilo->tid);
}

void *dump_memory_handler(void *args)
{
    t_tcb *hilo = (t_tcb *)args;
    int socket_cliente = iniciar_conexion_memoria();
    op_code resp = ejecutar_dump_memory(hilo, socket_cliente);
    finalizar_conexion_memoria(socket_cliente);

    if (resp == SUCCESS)
    {
        desencolar_estado_actual(hilo);
        encolar_en_ready(hilo);
    }
    else
    {
        log_error(logger_aux, "Error al realizar el dump de memoria del hilo <PID>: %d - <TID>: %d", hilo->pid, hilo->tid);
        process_exit_thread(hilo);
    }

    return NULL;
}

void dump_memory()
{
    t_tcb *hilo = hilo_en_exec;
    desencolar_estado_actual(hilo);
    encolar_en_blocked(hilo, M_DUMP_MEMORY);
    pthread_t hilo_dump;
    pthread_create(&hilo_dump, NULL, (void *)dump_memory_handler, (void *)hilo);
    pthread_detach(hilo_dump);
}

void *io_handler(void *args)
{
    t_io_params *io_params = (t_io_params *)args;
    pthread_mutex_lock(&mutex_io);
    usleep(io_params->tiempo * 1000);
    pthread_mutex_unlock(&mutex_io);
    desencolar_estado_actual(io_params->hilo);
    encolar_en_ready(io_params->hilo);
    log_info(logger, "## (%d:%d) finalizó IO y pasa a READY", io_params->hilo->pid, io_params->hilo->tid);
    free(io_params);
    return NULL;
}

void peticion_IO(int tiempo)
{
    t_tcb *hilo = hilo_en_exec;
    desencolar_estado_actual(hilo);
    encolar_en_blocked(hilo, M_IO);
    pthread_t hilo_io;
    t_io_params *io_params = malloc(sizeof(t_io_params));
    io_params->hilo = hilo;
    io_params->tiempo = tiempo;
    pthread_create(&hilo_io, NULL, (void *)io_handler, (void *)io_params);
    pthread_detach(hilo_io);
}

void iniciar_proceso(char *archivo, uint32_t prioridad, uint32_t tamanio)
{
    t_pcb *pcb = crear_pcb(archivo, tamanio, prioridad);
    log_info(logger,"## (%d:0) Se crea el proceso - Estado: NEW", pcb->pid);
}

void thread_create(char *archivo, uint32_t prioridad)
{
    t_tcb *hilo = hilo_en_exec;
    t_pcb *proceso = buscar_proceso(hilo->pid);
    t_tcb *nuevo_hilo = crear_tcb(proceso, archivo, prioridad);
    int socket_cliente = iniciar_conexion_memoria();
    op_code respuesta = crear_hilo_en_memoria(nuevo_hilo, socket_cliente);
    finalizar_conexion_memoria(socket_cliente);

    if (respuesta != SUCCESS)
    {
        log_error(logger_aux, "Error al crear el hilo en memoria <PID>: %d - <TID>: %d", nuevo_hilo->pid, nuevo_hilo->tid);
        process_exit_thread(nuevo_hilo);
        return;
    }
    else
    {
        log_info(logger, "## (%d:%d) - Se crea el hilo - Estado: READY", nuevo_hilo->pid, nuevo_hilo->tid);
        encolar_en_ready(nuevo_hilo);
    }
}

bool thread_join(uint32_t tid_bloqueador)
{
    t_tcb *hilo = hilo_en_exec;
    t_pcb *proceso = buscar_proceso(hilo->pid);
    t_tcb *hilo_bloqueador = buscar_hilo(proceso, tid_bloqueador);
    if (hilo_bloqueador != NULL)
    {
        if(hilo_bloqueador->estado != EXIT && hilo_bloqueador->tid == tid_bloqueador){
            desencolar_estado_actual(hilo);
            hilo->tid_bloqueador = tid_bloqueador;
            encolar_en_blocked(hilo, THREAD_JOIN);
            return false;
        }
        else {
            return true;
        }
    }
    else
    {
        log_warning(logger_aux, "El hilo bloqueador %d no existe en el proceso <PID>: %d", tid_bloqueador, hilo->pid);
    }
    return true;
}

void thread_cancel(uint32_t tid_cancelado)
{
    t_tcb *hilo = hilo_en_exec;
    t_pcb *proceso = buscar_proceso(hilo->pid);
    t_tcb *hilo_cancelado = buscar_hilo(proceso, tid_cancelado);
    if (hilo_cancelado != NULL)
    {
        desencolar_estado_actual(hilo_cancelado);
        log_info(logger_aux, "Proceso <PID>: %d - <Estado>: %s", hilo_cancelado->pid, obtener_estado_proceso(hilo_cancelado->estado));
        int socket_cliente = iniciar_conexion_memoria();
        op_code resp = finalizar_hilo_en_memoria(hilo_cancelado, socket_cliente);
        finalizar_conexion_memoria(socket_cliente);
        if (resp != SUCCESS)
        {
            log_error(logger_aux, "Error al finalizar el hilo en memoria <PID>: %d - <TID>: %d", hilo_cancelado->pid, hilo_cancelado->tid);
        }
        liberar_hilo(hilo_cancelado);
    }
    else
    {
        log_warning(logger_aux, "El hilo cancelado %d no existe en el proceso <PID>: %d", tid_cancelado, hilo->pid);
    }
}

void mutex_create(char *recurso)
{
    t_tcb *hilo = hilo_en_exec;
    t_pcb *proceso = buscar_proceso(hilo->pid);
    crear_recurso(recurso, proceso);
}

bool mutex_lock(char *recurso)
{
    t_tcb *hilo = hilo_en_exec;
    t_pcb *proceso = buscar_proceso(hilo->pid);
    t_recurso *recurso_buscado = buscar_recurso(recurso, proceso);
    if (recurso_buscado != NULL)
    {
        if (recurso_buscado->owner == -1)
        {
            recurso_buscado->owner = hilo->tid;
            log_info(logger_aux, "## MUTEX_LOCK: Recurso '%s' asignado a <PID>: %d - <TID>: %d", recurso, hilo->pid, hilo->tid);
            return true;
        }
        else
        {
            desencolar_estado_actual(hilo);
            encolar_en_blocked(hilo, MUTEX);
            log_info(logger_aux, "## MUTEX_LOCK: Recurso '%s' bloqueado por <PID>: %d - <TID>: %d", recurso, hilo->pid, hilo->tid);
            list_add(recurso_buscado->hilos, hilo);
            return false;
        }
    }
    else
    {
        log_warning(logger_aux, "El recurso %s no existe en el proceso <PID>: %d", recurso, hilo->pid);
        return true;
    }
}

void mutex_unlock(char *recurso)
{
    t_tcb *hilo = hilo_en_exec;
    t_pcb *proceso = buscar_proceso(hilo->pid);
    t_recurso *recurso_buscado = buscar_recurso(recurso, proceso);
    if (recurso_buscado != NULL)
    {
        if (recurso_buscado->owner == hilo->tid)
        {
            if (!list_is_empty(recurso_buscado->hilos))
            {
                t_tcb *nuevo_owner = list_remove(recurso_buscado->hilos, 0);
                recurso_buscado->owner = nuevo_owner->tid;
                log_info(logger_aux, "## MUTEX_UNLOCK: Recurso '%s' asignado a <PID>: %d - <TID>: %d", recurso, nuevo_owner->pid, nuevo_owner->tid);
                desencolar_estado_actual(nuevo_owner);
                encolar_en_ready(nuevo_owner);
            }
            else
            {
                recurso_buscado->owner = -1;
                log_info(logger_aux, "## MUTEX_UNLOCK: No hay hilos a asignar Recurso: '%s'", recurso);
            }
        }
        else
        {
            log_warning(logger_aux, "El hilo <PID>: %d - <TID>: %d no es el dueño del recurso %s", hilo->pid, hilo->tid, recurso);
        }
    }
    else
    {
        log_warning(logger_aux, "El recurso %s no existe en el proceso <PID>: %d", recurso, hilo->pid);
    }
}

void thread_exit()
{
    t_tcb *hilo = hilo_en_exec;
    desencolar_estado_actual(hilo);
    if (hilo->tid == 0)
    {
        process_exit();
    }
    else
    {
        int socket_cliente = iniciar_conexion_memoria();
        op_code resp = finalizar_hilo_en_memoria(hilo, socket_cliente);
        finalizar_conexion_memoria(socket_cliente);
        if (resp == SUCCESS)
        {
            liberar_hilo(hilo);
        }
        else
        {
            log_error(logger_aux, "Error al finalizar el hilo en memoria <PID>: %d - <TID>: %d", hilo->pid, hilo->tid);
        }
    }
}

void process_exit()
{
    t_tcb *hilo = hilo_en_exec;
    process_exit_thread(hilo);
}

void process_exit_thread(t_tcb *hilo)
{
    t_pcb *proceso = buscar_proceso(hilo->pid);
    desencolar_estado_actual(hilo);
    int socket_cliente = iniciar_conexion_memoria();
    op_code resp = finalizar_proceso_en_memoria(proceso, socket_cliente);
    finalizar_conexion_memoria(socket_cliente);
    if (resp == SUCCESS)
    {
        liberar_pcb(proceso);
        if (!largo_plazo)
        {
            largo_plazo = true;
        }
    }
    else
    {
        log_error(logger_aux, "Error al finalizar el proceso en memoria <PID>: %d", proceso->pid);
    }
}