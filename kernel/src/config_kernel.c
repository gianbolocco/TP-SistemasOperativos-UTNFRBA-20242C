#include "config_kernel.h"

void leer_configuracion()
{
    config = config_create("kernel.config");

    config_kernel = malloc(sizeof(t_config_kernel));
    config_kernel->IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    config_kernel->PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    config_kernel->IP_CPU = config_get_string_value(config, "IP_CPU");
    config_kernel->PUERTO_CPU_DISPATCH = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    config_kernel->PUERTO_CPU_INTERRUPT = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    config_kernel->ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    config_kernel->QUANTUM = config_get_int_value(config, "QUANTUM");
    config_kernel->LOG_LEVEL = config_get_string_value(config, "LOG_LEVEL");
}

void destroy_configuracion()
{
    if (config_kernel != NULL)
    {
        free(config_kernel);
    }
    if (config != NULL)
    {
        config_destroy(config);
    }
}