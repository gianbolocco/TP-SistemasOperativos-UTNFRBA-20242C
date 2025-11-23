#include "config_memoria.h"

void leer_configuracion()
{
    char *config_path = "./memoria.config";
    config = config_create(config_path);
    if (config == NULL)
    {
        log_error(logger, "No se pudo leer el archivo de configuracion");
        return;
    }
    // Reservo espacio de memoria para la estructura de datos
    config_datos = malloc(sizeof(t_config_memoria));
    config_datos->puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    config_datos->ip_filesystem = config_get_string_value(config, "IP_FILESYSTEM");
    config_datos->puerto_filesystem = config_get_string_value(config, "PUERTO_FILESYSTEM");
    config_datos->tamanio_memoria = config_get_int_value(config, "TAM_MEMORIA");
    config_datos->path_instrucciones = config_get_string_value(config, "PATH_INSTRUCCIONES");
    config_datos->retardo_respuesta = config_get_int_value(config, "RETARDO_RESPUESTA");
    config_datos->esquema = config_get_string_value(config, "ESQUEMA");
    config_datos->algoritmo_de_busqueda = config_get_string_value(config, "ALGORITMO_BUSQUEDA");
    config_datos->particiones = config_get_array_value(config, "PARTICIONES"); // aca tengo mis particioens el tamaño q me dan
    config_datos->log_level = config_get_string_value(config, "LOG_LEVEL");
}

void destroy_configuracion()
{
    if (config_datos != NULL)
    {
        string_array_destroy(config_datos->particiones);
        free(config_datos);
    }
    if (config != NULL)
    {
        config_destroy(config);
    }
}