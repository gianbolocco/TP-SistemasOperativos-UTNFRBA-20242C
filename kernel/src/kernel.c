#include "kernel.h"

void inicializar_kernel()
{
    cola_new = list_create();
    cola_ready = list_create();
    cola_blocked = list_create();
    procesos_en_sistema = list_create();

    sem_init(&hay_pcbs_en_new, 0, 0);
    sem_init(&hay_pcbs_en_ready, 0, 0);
    sem_init(&hay_pcbs_en_blocked, 0, 0);

    pthread_mutex_init(&mutex_new, NULL);
    pthread_mutex_init(&mutex_ready, NULL);
    pthread_mutex_init(&mutex_blocked, NULL);

    pthread_mutex_init(&mutex_io, NULL);

    iniciar_conexion_cpu_interrupt();
    iniciar_conexion_cpu_dispath();
}

void finalizar_kernel()
{
    list_destroy(cola_new);
    list_destroy(cola_ready);
    list_destroy(cola_blocked);
    for(int i = 0; i < list_size(procesos_en_sistema); i++)
    {
        t_pcb* proceso = list_get(procesos_en_sistema, i);
        if (proceso->threads != NULL)
        {
            if (list_size(proceso->threads) > 0)
            {
                process_exit_thread(list_get(proceso->threads, 0));
            }
        }
    }
    list_destroy(procesos_en_sistema);

    sem_destroy(&hay_pcbs_en_new);
    sem_destroy(&hay_pcbs_en_ready);
    sem_destroy(&hay_pcbs_en_blocked);

    pthread_mutex_destroy(&mutex_new);
    pthread_mutex_destroy(&mutex_ready);
    pthread_mutex_destroy(&mutex_blocked);

    pthread_mutex_destroy(&mutex_io);
    finalizar_conexion_cpu();
}