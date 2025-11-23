#include "utils.h"

char *preparar_ruta(char *modulo, char *extension)
{

    char *path = string_new();
    string_append(&path, "./");
    string_append(&path, modulo);
    string_append(&path, ".");
    string_append(&path, extension);

    return path;
}

t_log *iniciar_logger(char *modulo, t_log_level log_level)
{
    char *path = preparar_ruta(modulo, "log");
    t_log *nuevo_logger = log_create(path, "B.E.G.G", true, log_level);

    return nuevo_logger;
}

t_config *iniciar_config(char *modulo)
{
    char *path = preparar_ruta(modulo, "config");
    t_config *nuevo_config = config_create(path);

    return nuevo_config;
}

char *obtener_codigo_operacion(op_code codigo_operacion)
{
    char *cod_op;
    switch (codigo_operacion)
    {

    case MENSAJE:
        cod_op = "MENSAJE";
        break;
    case PAQUETE:
        cod_op = "PAQUETE";
        break;
    case EJECUCION_HILO:
        cod_op = "EJECUCION_HILO";
        break;
    case INSTRUCCIONES:
        cod_op = "INSTRUCCIONES";
        break;
    case PCONTEXTO:
        cod_op = "PCONTEXTO";
        break;
    case PCONTEXTODESALOJO:
        cod_op = "PCONTEXTODESALOJO";
        break;
    case FIN_DE_CLOCK:
        cod_op = "FIN_DE_CLOCK";
        break;
    case INSTRUCCION:
        cod_op = "INSTRUCCION";
        break;
    case INFO:
        cod_op = "INFO";
        break;
    case CREAR_PROCESO:
        cod_op = "CREAR_PROCESO";
        break;
    case FIN_PROCESO:
        cod_op = "FIN_PROCESO";
        break;
    case CREAR_HILO:
        cod_op = "CREAR_HILO";
        break;
    case FIN_HILO:
        cod_op = "FIN_HILO";
        break;
    case MEMORY_DUMP:
        cod_op = "MEMORY_DUMP";
        break;
    case OBTENER_CONTEXTO:
        cod_op = "OBTENER_CONTEXTO";
        break;
    case ACTUALIZAR_CONTEXTO:
        cod_op = "ACTUALIZAR_CONTEXTO";
        break;
    case OBTENER_INSTRUCCION:
        cod_op = "OBTENER_INSTRUCCION";
        break;
    case READ_MEM:
        cod_op = "READ_MEM";
        break;
    case WRITE_MEM:
        cod_op = "WRITE_MEM";
        break;
    case MAX_SIZE_ERROR:
        cod_op = "MAX_SIZE_ERROR";
        break;
    case SIZE_ERROR:
        cod_op = "SIZE_ERROR";
        break;
    case PATH_ERROR:
        cod_op = "PATH_ERROR";
        break;
    case SUCCESS:
        cod_op = "SUCCESS";
        break;
    case MOTIVO_DESALOJO:
        cod_op = "MOTIVO_DESALOJO";
        break;
    default:
        break;
    }

    return cod_op;
}