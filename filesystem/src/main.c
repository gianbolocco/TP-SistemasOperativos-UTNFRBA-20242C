#include "main.h"

int main(void)
{
    // Para cierre seguro
    //signal(SIGINT, end_filesystem_server);

    // INICIALIZAR CONFIGURACIÓN
    config = read_config("filesystem.config");
    if (config == NULL)
    {
        log_error(logger_aux, "No se pudo leer el archivo de configuración");
        log_destroy(logger_aux);
        return EXIT_FAILURE;
    }

    // INICIALIZAR LOGS
    logger = log_create("filesystem.log", "FILESYSTEM", 1, log_level_from_string(config->log_level));
    logger_aux = log_create("filesystem_aux.log", "FILESYSTEM AUX", 1, LOG_LEVEL_DEBUG);

    iniciar_fs();
    // INICIALIZAR SERVIDOR
    start_filesystem_server();

    // ESPERAR CONEXIONES

    while (!end_program_flag)
    {
        sleep(1);
    }

    end_filesystem_server();
    return EXIT_SUCCESS;
}
