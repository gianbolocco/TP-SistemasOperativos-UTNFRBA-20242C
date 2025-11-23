#include "main.h"

int main()
{
    signal(SIGINT, sigintHandler);
    read_config();
    if (config_cpu == NULL)
    {
        return EXIT_FAILURE;
    }
    t_log_level nivel_log = log_level_from_string(config_cpu->log_level);
    logger = log_create("cpu.log", "CPU", true, nivel_log);
    logger_aux = log_create("cpu_aux.log", "CPU AUX", true, LOG_LEVEL_TRACE);

    iniciar_conexion_memoria();
    if (socket_memoria == -1)
    {
        log_error(logger, "No se pudo conectar con la memoria");
        destroy_config();
        log_destroy(logger);
        log_destroy(logger_aux);
        return EXIT_FAILURE;
    }

    iniciar_servidor_cpu();

    while (!flag_fin_programa)
    {
        sleep(1);
    }

    pthread_join(thr_server_dispatch, NULL);
    pthread_join(thr_server_interrupt, NULL);

    destroy_contexto();
    destroy_config();
    finalizar_conexion_memoria();
    finalizar_servidor_cpu();
    log_destroy(logger);
    log_destroy(logger_aux);

    return EXIT_SUCCESS;
}

void sigintHandler(int signum)
{
    log_warning(logger_aux, "Iniciando fin del modulo por signal de apagado");
    flag_fin_programa = true;
    //pthread_join(thr_server_dispatch, NULL);
    //pthread_join(thr_server_interrupt, NULL);
    log_warning(logger_aux, "hilos finalizados");
    destroy_contexto();
    destroy_config();
    finalizar_conexion_memoria();
    finalizar_servidor_cpu();
    log_destroy(logger);
    log_destroy(logger_aux);
    exit(signum);
}
