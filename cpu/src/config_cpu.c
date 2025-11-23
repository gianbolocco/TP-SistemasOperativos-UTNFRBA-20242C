#include "config_cpu.h"

void read_config()
{
    char *path = "./cpu.config";
    t_config *config = config_create(path);
    if (config == NULL)
    {
        log_error(logger, "No se pudo leer el archivo de configuracion");
        return;
    }

    config_cpu = malloc(sizeof(t_config_cpu));
    config_cpu->ip_memoria = string_duplicate(config_get_string_value(config, "IP_MEMORIA"));
    config_cpu->puerto_memoria = string_itoa(config_get_int_value(config, "PUERTO_MEMORIA"));
    config_cpu->puerto_dispatch = string_itoa(config_get_int_value(config, "PUERTO_ESCUCHA_DISPATCH"));
    config_cpu->puerto_interrupt = string_itoa(config_get_int_value(config, "PUERTO_ESCUCHA_INTERRUPT"));
    config_cpu->log_level = string_duplicate(config_get_string_value(config, "LOG_LEVEL"));

    config_destroy(config);
}

void destroy_config()
{
    if (config_cpu == NULL)
    {
        return;
    }
    free(config_cpu->ip_memoria);
    free(config_cpu->puerto_memoria);
    free(config_cpu->puerto_dispatch);
    free(config_cpu->puerto_interrupt);
    free(config_cpu->log_level);
    free(config_cpu);
}