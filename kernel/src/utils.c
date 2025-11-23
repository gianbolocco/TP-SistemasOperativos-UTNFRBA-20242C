#include "utils.h"

t_pcb *crear_pcb(char *archivo, int tamanio, int prioridad)
{
    t_pcb *pcb = malloc(sizeof(t_pcb));
    pcb->pid = pid_counter;
    pcb->threads = list_create();
    pcb->mutex = list_create();
    pcb->estado = NEW;
    pcb->tamanio = tamanio;
    pcb->tid_contador = 0;

    pid_counter++;
    list_add(procesos_en_sistema, pcb);
    crear_tcb(pcb, archivo, prioridad);
    encolar_en_new(pcb);

    return pcb;
}

t_tcb *crear_tcb(t_pcb *proceso, char *archivo, int prioridad)
{
    t_tcb *tcb = malloc(sizeof(t_tcb));
    tcb->pid = proceso->pid;
    tcb->tid = proceso->tid_contador;
    tcb->prioridad = prioridad;
    tcb->estado = READY;
    tcb->archivo = string_duplicate(archivo);
    tcb->tid_bloqueador = -1;
    tcb->motivo_bloqueo = NONE;
    tcb->temporal = NULL;
    proceso->tid_contador++;
    list_add(proceso->threads, tcb);

    return tcb;
}

t_recurso *crear_recurso(char *nombre, t_pcb *proceso)
{
    t_recurso *recurso = malloc(sizeof(t_recurso));
    recurso->nombre = string_duplicate(nombre);
    recurso->hilos = list_create();
    recurso->owner = -1;

    list_add(proceso->mutex, recurso);

    return recurso;
}

void liberar_pcb(t_pcb *proceso)
{
    log_info(logger, "## Finaliza el proceso %d", proceso->pid);
    log_debug(logger_aux, "Liberando proceso <PID>: %d", proceso->pid);
    if (proceso->threads != NULL)
    {
        for(int i = 0; i < list_size(proceso->threads); i++){
            desencolar_estado_actual(list_get(proceso->threads,i));
        }
        for(int i = 0; i < list_size(proceso->threads); i++){
            t_tcb *hilo = list_get(proceso->threads,i);
            list_remove_element(proceso->threads,hilo);
            liberar_hilo(hilo);
        }
        list_destroy(proceso->threads);
        proceso->threads = NULL;
    }
    if (proceso->mutex != NULL)
    {
        for(int i = 0; i < list_size(proceso->mutex); i++){
            t_recurso *recurso = list_get(proceso->mutex,i);
            list_remove_element(proceso->mutex,recurso);
            liberar_recurso(recurso);
        }
        list_destroy(proceso->mutex);
        proceso->mutex = NULL;
    }
    list_remove_element(procesos_en_sistema, proceso);
    free(proceso);
}

void liberar_hilo(t_tcb *hilo)
{
    log_info(logger, "## (%d:%d) Finaliza el hilo", hilo->pid, hilo->tid);
    log_debug(logger_aux, "Liberando hilo <PID>: %d - <TID>: %d", hilo->pid, hilo->tid);
    t_pcb *proceso = buscar_proceso(hilo->pid);
    if (hilo->archivo != NULL)
    {
        free(hilo->archivo);
    }
    if (proceso != NULL)
    {
        list_remove_element(proceso->threads, hilo);
    }
    for(int i = 0; i < list_size(cola_blocked); i++)
    {
        // Busco si hay hilos bloqueados por join
        t_tcb *hilo_bloqueado = list_get(cola_blocked,i);
        if (hilo_bloqueado->pid == hilo->pid && hilo_bloqueado->tid_bloqueador == hilo->tid && hilo_bloqueado->motivo_bloqueo == THREAD_JOIN)
        {
            hilo_bloqueado->tid_bloqueador = -1;
            hilo_bloqueado->motivo_bloqueo = NONE;
            desencolar_estado_actual(hilo_bloqueado);
            encolar_en_ready(hilo_bloqueado);
        }
    }    
    if(proceso != NULL){
        for(int i = 0; i < list_size(proceso->mutex); i++)
        {
            t_recurso *recurso = list_get(proceso->mutex,i);
            if (recurso->owner == hilo->tid)
            {
                if (list_size(recurso->hilos) > 0)
                {
                    t_tcb * nuevo_owner = list_get(recurso->hilos,0);
                    recurso->owner = nuevo_owner->tid;
                    desencolar_estado_actual(nuevo_owner);
                    encolar_en_ready(nuevo_owner);
                } else {
                    recurso->owner = -1;
                }
            }
        }
    }
    
    if (hilo->temporal != NULL)
    {
        temporal_destroy(hilo->temporal);
    }
    free(hilo);
}

void liberar_recurso(t_recurso *recurso)
{
    if (recurso->nombre != NULL)
    {
        free(recurso->nombre);
        recurso->nombre = NULL;
    }
    list_destroy(recurso->hilos);
    free(recurso);
}

t_pcb *buscar_proceso(uint32_t pid)
{
    t_pcb *proceso_buscado = NULL;
    for (int i = 0; i < list_size(procesos_en_sistema); i++)
    {
        proceso_buscado = (t_pcb *)list_get(procesos_en_sistema, i);
        if (proceso_buscado != NULL && proceso_buscado->pid == pid)
        {
            break;
        }
    }
    return proceso_buscado;
}

t_tcb *buscar_hilo(t_pcb *proceso, uint32_t param_tid)
{
    t_tcb *hilo_buscado = NULL;
    for (int i = 0; i < list_size(proceso->threads); i++)
    {
        hilo_buscado = (t_tcb *)list_get(proceso->threads, i);
        if (hilo_buscado->tid == param_tid)
        {
            break;
        }
    }
    return hilo_buscado;
}

t_recurso *buscar_recurso(char *nombre, t_pcb *proceso)
{
    t_recurso *recurso_buscado = NULL;
    for (int i = 0; i < list_size(proceso->mutex); i++)
    {
        recurso_buscado = (t_recurso *)list_get(proceso->mutex, i);
        if (string_equals_ignore_case(recurso_buscado->nombre, nombre))
        {
            break;
        }
    }
    return recurso_buscado;
}

void encolar_en_new(t_pcb *proceso)
{
    pthread_mutex_lock(&mutex_new);
    list_add(cola_new, proceso);
    proceso->estado = NEW;
    pthread_mutex_unlock(&mutex_new);
    log_info(logger, "Proceso <PID>: %d - <Estado>: %s", proceso->pid, obtener_estado_proceso(proceso->estado));
    sem_post(&hay_pcbs_en_new);
}

void encolar_en_ready(t_tcb *hilo)
{
    pthread_mutex_lock(&mutex_ready);
    list_add(cola_ready, hilo);
    hilo->estado = READY;
    if (hilo->temporal != NULL)
    {
        temporal_destroy(hilo->temporal);
    }
    hilo->temporal = temporal_create();
    pthread_mutex_unlock(&mutex_ready);
    log_info(logger, "Proceso <PID>: %d - <Estado>: %s", hilo->pid, obtener_estado_proceso(hilo->estado));
    sem_post(&hay_pcbs_en_ready);
}

void encolar_en_blocked(t_tcb *hilo, t_motivo_bloqueo motivo_bloqueo)
{
    pthread_mutex_lock(&mutex_blocked);
    list_add(cola_blocked, hilo);
    hilo->estado = BLOCKED;
    hilo->motivo_bloqueo = motivo_bloqueo;
    pthread_mutex_unlock(&mutex_blocked);
    log_info(logger, "## (%d:%d)- Bloqueado por: <%s>", hilo->pid, hilo->tid, obtener_motivo_bloqueo(hilo->motivo_bloqueo));
    log_info(logger_aux, "Proceso <PID>: %d - <Estado>: %s", hilo->pid, obtener_estado_proceso(hilo->estado));
    sem_post(&hay_pcbs_en_blocked);
}

void desencolar_estado_actual(t_tcb *hilo)
{
    switch (hilo->estado)
    {
    case NEW:
        pthread_mutex_lock(&mutex_new);
        list_remove_element(cola_new, hilo);
        pthread_mutex_unlock(&mutex_new);
        break;
    case READY:
        pthread_mutex_lock(&mutex_ready);
        list_remove_element(cola_ready, hilo);
        pthread_mutex_unlock(&mutex_ready);
        break;
    case EXEC:
        hilo_en_exec = NULL;
        break;
    case BLOCKED:
        pthread_mutex_lock(&mutex_blocked);
        list_remove_element(cola_blocked, hilo);
        pthread_mutex_unlock(&mutex_blocked);
        break;
    case EXIT:
        break;
    default:
        break;
    }
}

char *obtener_estado_proceso(t_estado estado)
{
    char *estado_str;
    switch (estado)
    {
    case NEW:
        estado_str = "NEW";
        break;
    case READY:
        estado_str = "READY";
        break;
    case EXEC:
        estado_str = "EXEC";
        break;
    case BLOCKED:
        estado_str = "BLOCKED";
        break;
    case EXIT:
        estado_str = "EXIT";
        break;
    }

    return estado_str;
}

char *obtener_motivo_bloqueo(t_motivo_bloqueo motivo_bloqueo)
{
    char *motivo_string;
    switch (motivo_bloqueo)
    {
    case THREAD_JOIN:
        motivo_string = "PTHREAD_JOIN";
        break;
    case MUTEX:
        motivo_string = "MUTEX";
        break;
    case IO:
        motivo_string = "IO";
        break;
    case M_DUMP:
        motivo_string = "DUMP_MEMORY";
    default:
        break;
    }

    return motivo_string;
}

char *nombre_syscall(t_motivo_desalojo motivo)
{
    char *nombre;
    switch (motivo)
    {
    case M_DUMP_MEMORY:
        nombre = "DUMP_MEMORY";
        break;
    case M_IO:
        nombre = "IO";
        break;
    case M_PROCESS_CREATE:
        nombre = "PROCESS_CREATE";
        break;
    case M_PROCESS_EXIT:
        nombre = "PROCESS_EXIT";
        break;
    case M_THREAD_CREATE:
        nombre = "THREAD_CREATE";
        break;
    case M_THREAD_JOIN:
        nombre = "THREAD_JOIN";
        break;
    case M_THREAD_CANCEL:
        nombre = "THREAD_CANCEL";
        break;
    case M_THREAD_EXIT:
        nombre = "THREAD_EXIT";
        break;
    case M_MUTEX_CREATE:
        nombre = "MUTEX_CREATE";
        break;
    case M_MUTEX_LOCK:
        nombre = "MUTEX_LOCK";
        break;
    case M_MUTEX_UNLOCK:
        nombre = "MUTEX_UNLOCK";
        break;
    case CONTINUE:
        nombre = "CONTINUE";
        break;
    case INTERRUPCION:
        nombre = "INTERRUPCION";
        break;
    case SEGFAULT:
        nombre = "SEGFAULT";
        break;
    default:
        nombre = "NO DEFINIDO";
        break;
    }
    return nombre;
}