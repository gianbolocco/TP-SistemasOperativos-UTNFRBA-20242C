#include "planificador_largo_plazo.h"

void inicializar_planificador_largo_plazo()
{
    log_info(logger_aux, "Inicializando planificador de largo plazo");
    pthread_create(&thr_largo_plazo, NULL, (void *)ejecutar_planificador_largo_plazo, NULL);
}

void finalizar_planificador_largo_plazo()
{
    flag_planificador_largo_plazo_finalizado = true;
    sem_post(&hay_pcbs_en_new);
    pthread_join(thr_largo_plazo, NULL);
    log_info(logger_aux, "Planificador de largo plazo finalizado");
}

void *ejecutar_planificador_largo_plazo()
{
    while (!flag_fin_de_programa && !flag_planificador_largo_plazo_finalizado)
    {
        if (!largo_plazo)
        {
            sleep(1);
            continue;
        }
        sem_wait(&hay_pcbs_en_new);
        if (flag_fin_de_programa || flag_planificador_largo_plazo_finalizado)
        {
            break;
        }

        t_pcb *proceso = list_get(cola_new, 0);
        t_tcb *hilo = list_get(proceso->threads, 0);

        /*
        if(!list_is_empty(proceso->threads)){
            hilo = list_get(proceso->threads, 0);
        }
        */

        int socket_cliente = iniciar_conexion_memoria();
        op_code respuesta = crear_proceso_en_memoria(hilo, socket_cliente);
        finalizar_conexion_memoria(socket_cliente);

        switch (respuesta)
        {
        case SUCCESS:
            log_info(logger_aux, "Proceso <PID>: %d - Creado en memoria", proceso->pid);
            pthread_mutex_lock(&mutex_new);
            list_remove_element(cola_new, proceso);
            pthread_mutex_unlock(&mutex_new);
            proceso->estado = READY;
            encolar_en_ready(hilo);
            log_debug(logger_aux, "Proceso <PID>: %d - <Estado Anterior>: NEW - <Estado Nuevo>: READY", proceso->pid);
            break;
        case MAX_SIZE_ERROR:
            log_error(logger_aux, "Error al crear el proceso <PID>: %d - Tamaño maximo de programa excedido", proceso->pid);
            pthread_mutex_lock(&mutex_new);
            list_remove_element(cola_new, proceso);
            pthread_mutex_unlock(&mutex_new);
            proceso->estado = EXIT;
            liberar_pcb(proceso);
            break;
        case PATH_ERROR:
            log_error(logger_aux, "Error al crear el proceso <PID>: %d - Path invalido", proceso->pid);
            pthread_mutex_lock(&mutex_new);
            list_remove_element(cola_new, proceso);
            pthread_mutex_unlock(&mutex_new);
            proceso->estado = EXIT;
            liberar_pcb(proceso);
            break;
        case SIZE_ERROR:
            log_error(logger_aux, "Error al crear el proceso <PID>: %d - No hay espacio disponible actualmente", proceso->pid);
            largo_plazo = false;
            sem_post(&hay_pcbs_en_new);
            break;
        default:
            log_error(logger_aux, "Error al crear el proceso <PID>: %d - Error desconocido", proceso->pid);
            pthread_mutex_lock(&mutex_new);
            list_remove_element(cola_new, proceso);
            pthread_mutex_unlock(&mutex_new);
            proceso->estado = EXIT;
            liberar_pcb(proceso);
            break;
        }
    }

    return NULL;
}