#include "main.h"

int main(int argc, char *argv[])
{
    // Para cierre seguro
    signal(SIGINT, sigintHandler);

    leer_configuracion();
    if (config_kernel == NULL)
    {
        log_error(logger_aux, "Error al leer configuracion");
        return EXIT_FAILURE;
    }

    logger_aux = log_create("kernel_aux.log", "KERNEL AUX", true, LOG_LEVEL_TRACE);
    t_log_level nivel_log = log_level_from_string(config_kernel->LOG_LEVEL);
    logger = log_create("kernel.log", "KERNEL", true, nivel_log);

    if (argc < 2)
    {
        log_error(logger_aux, "Faltan argumentos");
        log_destroy(logger_aux);
        log_destroy(logger);
        destroy_configuracion();
        return EXIT_FAILURE;
    }

    inicializar_kernel();

    for (int i = 1; i < argc; i += 2)
    {
        char *archivo = argv[i];
        int32_t tamanio = atoi(argv[i + 1]);
        log_info(logger_aux, "Procesando archivo %s con tamaño de memoria %d", archivo, tamanio);
        iniciar_proceso(archivo, 0, tamanio);
    }

    inicializar_planificador_corto_plazo();
    inicializar_planificador_largo_plazo();

    while (!flag_fin_de_programa)
    {
        sleep(1);
    }

    log_info(logger_aux, "Finalizando programa");

    finalizar_programa();

    return EXIT_SUCCESS;
}

void sigintHandler(int signum)
{
    log_warning(logger_aux, "Iniciando fin del modulo por signal de apagado");
    finalizar_programa();
    exit(signum);
}

void finalizar_programa()
{
    log_info(logger_aux, "Cerrando programa");
    finalizar_planificador_corto_plazo();
    finalizar_planificador_largo_plazo();
    finalizar_kernel();
    destroy_configuracion();

    log_destroy(logger_aux);
    log_destroy(logger);
}