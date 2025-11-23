#include "planificador_corto_plazo.h"

void inicializar_planificador_corto_plazo()
{
    log_info(logger_aux, "Inicializando planificador de corto plazo");
    pthread_create(&thr_corto_plazo, NULL, (void *)ejecutar_planificador_corto_plazo, NULL);
}

void finalizar_planificador_corto_plazo()
{
    flag_planificador_corto_plazo_finalizado = true;
    sem_post(&hay_pcbs_en_ready);
    pthread_join(thr_corto_plazo, NULL);
    log_info(logger_aux, "Planificador de corto plazo finalizado");
}

void *ejecutar_planificador_corto_plazo()
{
    while (!flag_fin_de_programa && !flag_planificador_corto_plazo_finalizado)
    {
        sem_wait(&hay_pcbs_en_ready);

        if (flag_fin_de_programa || flag_planificador_corto_plazo_finalizado)
        {
            break;
        }

        if (list_size(cola_ready) == 0)
        {
            continue;
        }

        t_tcb *hilo = planificar_corto_plazo();
        if (hilo == NULL)
        {
            log_error(logger_aux, "No se pudo planificar el hilo");
            process_exit_thread(hilo_en_exec);
            continue;
        }
        hilo->estado = EXEC;
        hilo_en_exec = hilo;
        log_debug(logger_aux, "Proceso <PID>: %d - Hilo <TID>: %d - <Estado Anterior>: READY - <Estado Nuevo>: EXEC", hilo->pid, hilo->tid);

        // LUEGO DE EJECUTAR EL HILO LA FUNCION MISMA SETEA EL NUEVO ESTADO Y LO MUEVE A LA COLA CORRESPONDIENTE
        if (string_equals_ignore_case(config_kernel->ALGORITMO_PLANIFICACION, "CMN"))
        {
            flag_ejecutando_en_cpu = true;
            pthread_create(&hilo_quantum, NULL, (void *)rr_handler, NULL);
            // pthread_detach(hilo_quantum);
            log_warning(logger_aux, "Hilo quantum creado %d:%d", hilo_en_exec->pid, hilo_en_exec->tid);
        }
        ejecutar_hilo(true);
        flag_ejecutando_en_cpu = false;
        hilo_en_exec = NULL;
        flag_desalojar=false;
        if (string_equals_ignore_case(config_kernel->ALGORITMO_PLANIFICACION, "CMN"))
        {
            pthread_join(hilo_quantum, NULL);
        }
    }
    return NULL;
}

bool manejar_cambio_estado(t_tcb *hilo)
{
    if (flag_fin_de_programa || flag_planificador_corto_plazo_finalizado)
    {
        desencolar_estado_actual(hilo);
        encolar_en_ready(hilo);
        return false; // No procesar más
    }

    if (flag_desalojar)
    {
        log_info(logger, "Desalojado por fin de Quantum (%d:%d)", hilo->pid, hilo->tid);
        flag_desalojar = false;
        desencolar_estado_actual(hilo);
        encolar_en_ready(hilo);
        return false;
    }

    return true; // Hilo puede seguir ejecutándose
}

void ejecutar_hilo(bool first_run)
{
    // Para que no se ejecute el hilo si se finalizo el programa
    if (!manejar_cambio_estado(hilo_en_exec))
    {
        return;
    }

    log_debug(logger_aux, "Ejecutando hilo <PID>: %d - <TID>: %d", hilo_en_exec->pid, hilo_en_exec->tid);
    enviar_hilo_a_cpu();

    t_package *paquete = recibir_paquete(socket_cpu_dispatch);
    if (paquete->codigo_operacion != MOTIVO_DESALOJO)
    {
        log_error(logger_aux, "Error en la respuesta de la CPU - PID: %d - TID: %d", hilo_en_exec->pid, hilo_en_exec->tid);
        process_exit();
        return;
    }

    t_list *datos = deserealizar_paquete(paquete);
    // uint32_t pid = *((uint32_t *)list_get(datos, 0));
    // uint32_t tid = *((uint32_t *)list_get(datos, 1));
    t_motivo_desalojo motivo_desalojo = *((t_motivo_desalojo *)list_get(datos, 2));

    char *param_archivo = NULL;
    uint32_t param_tamanio = 0;
    uint32_t param_prioridad = 0;
    uint32_t tid_param = 0;
    char *recurso_param = NULL;

    if (motivo_desalojo != SEGFAULT && motivo_desalojo != CONTINUE)
    {
        log_info(logger, "## (%d:%d) - Solicitó syscall: <%s>", hilo_en_exec->pid, hilo_en_exec->tid, nombre_syscall(motivo_desalojo));
    }
    else if (motivo_desalojo == INTERRUPCION)
    {
        log_info(logger, "## (%d:%d) - Desalojado por fin de Quantum", hilo_en_exec->pid, hilo_en_exec->tid);
    }
    else
    {
        log_debug(logger_aux, "Motivo desalojo: %s", nombre_syscall(motivo_desalojo));
    }

    switch (motivo_desalojo)
    {
    case INTERRUPCION:
        log_debug(logger_aux, "El hilo fue desalojado por una interrupcion");
        interrupcion();
        break;
    case M_DUMP_MEMORY:
        dump_memory();
        break;
    case M_IO:
        int tiempo = atoi((char *)list_get(datos, 3));
        peticion_IO(tiempo);
        break;
    case M_PROCESS_CREATE:
        param_archivo = (char *)list_get(datos, 3);
        param_tamanio = atoi((char *)list_get(datos, 4));
        param_prioridad = atoi((char *)list_get(datos, 5));
        iniciar_proceso(param_archivo, param_prioridad, param_tamanio);
        ejecutar_hilo(false);
        break;
    case M_THREAD_CREATE:
        param_archivo = (char *)list_get(datos, 3);
        param_prioridad = atoi((char *)list_get(datos, 4));
        thread_create(param_archivo, param_prioridad);
        ejecutar_hilo(false);
        break;
    case M_THREAD_JOIN:
        tid_param = atoi((char *)list_get(datos, 3));
        if (thread_join(tid_param))
        {
            ejecutar_hilo(false);
        }
        break;
    case M_THREAD_CANCEL:
        tid_param = atoi((char *)list_get(datos, 3));
        thread_cancel(tid_param);
        break;
    case M_MUTEX_CREATE:
        recurso_param = (char *)list_get(datos, 3);
        mutex_create(recurso_param);
        ejecutar_hilo(false);
        break;
    case M_MUTEX_LOCK:
        recurso_param = (char *)list_get(datos, 3);
        bool res = mutex_lock(recurso_param);
        if (res)
        {
            ejecutar_hilo(false);
        }
        break;
    case M_MUTEX_UNLOCK:
        recurso_param = (char *)list_get(datos, 3);
        mutex_unlock(recurso_param);
        ejecutar_hilo(false);
        break;
    case M_THREAD_EXIT:
        thread_exit();
        break;
    case M_PROCESS_EXIT:
        process_exit();
        break;
    default:
        log_error(logger_aux, "El motivo de desalojo no es valido: %d", motivo_desalojo);
        process_exit();
        break;
    }

    list_destroy_and_destroy_elements(datos, free);
    eliminar_paquete(paquete);
}