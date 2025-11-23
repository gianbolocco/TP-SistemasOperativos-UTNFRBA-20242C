#include "algoritmos_planificacion.h"

t_tcb *planificar_corto_plazo()
{
    if (string_equals_ignore_case(config_kernel->ALGORITMO_PLANIFICACION, "FIFO"))
    {
        return algoritmo_fifo();
    }
    else if (string_equals_ignore_case(config_kernel->ALGORITMO_PLANIFICACION, "PRIORIDADES"))
    {
        return algoritmo_prioridades();
    }
    else if (string_equals_ignore_case(config_kernel->ALGORITMO_PLANIFICACION, "CMN"))
    {
        return algoritmo_prioridades();
    }
    else
    {
        log_info(logger, "Algoritmo incorrecto");
        flag_fin_de_programa = 1;
        return NULL;
    }
}

t_tcb *algoritmo_fifo()
{
    pthread_mutex_lock(&mutex_ready);
    t_tcb *hilo = (t_tcb *)list_remove(cola_ready, 0);
    pthread_mutex_unlock(&mutex_ready);

    return hilo;
}

bool mayor_prioridad_nuevo(t_tcb *hilo, t_tcb *nuevo_pcb)
{
    t_temporal *temporal = temporal_create();
    int64_t tiempo_espera_nuevo_pcb = temporal_diff(nuevo_pcb->temporal, temporal);
    int64_t tiempo_espera_hilo = temporal_diff(hilo->temporal, temporal);
    //log_debug(logger_aux, "PRIORIDAD: %d Tiempo de espera nuevo TCB %d %d: %ld", nuevo_pcb->prioridad, nuevo_pcb->pid, nuevo_pcb->tid, tiempo_espera_nuevo_pcb);
    //log_debug(logger_aux, "PRIORIDAD: %d Tiempo de espera hilo %d %d: %ld", hilo->prioridad, hilo->pid, hilo->tid, tiempo_espera_hilo);
    temporal_destroy(temporal);
    if ((nuevo_pcb->prioridad) < (hilo->prioridad))
    {
        return true;
    }
    else if ((nuevo_pcb->prioridad) == (hilo->prioridad))
    {
        if (tiempo_espera_nuevo_pcb > tiempo_espera_hilo)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

t_tcb *algoritmo_prioridades()
{
    int cant_procesos = list_size(cola_ready);
    int mayorPrioridad = 0;

    for (int i = 1; i < cant_procesos; i++)
    {
        if (mayor_prioridad_nuevo(list_get(cola_ready, mayorPrioridad), list_get(cola_ready, i)))
        {
            mayorPrioridad = i;
        }
    }
    pthread_mutex_lock(&mutex_ready);
    t_tcb *hilo = (t_tcb *)list_remove(cola_ready, mayorPrioridad);
    pthread_mutex_unlock(&mutex_ready);

    return hilo;
}

void *rr_handler(void *args)
{
    t_tcb *hilo = hilo_en_exec;
    if(hilo == NULL){
        return NULL;
    }
    uint32_t ppid = hilo->pid;
    uint32_t ttid = hilo->tid;
    log_warning(logger_aux, "ENTRE AL HANDLER %d:%d", ppid, ttid);
    usleep(config_kernel->QUANTUM * 1000);
    if (hilo_en_exec != NULL)
    {
        log_warning(logger_aux, "Hilo quantum: ejecutando ahora %d:%d", hilo_en_exec->pid, hilo_en_exec->tid);
    }
    if (hilo_en_exec != NULL && flag_ejecutando_en_cpu && hilo_en_exec->tid == ttid && hilo_en_exec->pid == ppid)
    {
        flag_desalojar = true;
        log_warning(logger_aux, "## (%d:%d) - Desalojado por fin de Quantum", ppid, ttid);
        enviar_desalojo_a_cpu();
    }
    log_warning(logger_aux, "Hilo quantum finalizado %d:%d", ppid, ttid);
    return NULL;
}