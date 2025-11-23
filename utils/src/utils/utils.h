#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include "./utils/estructuras_globales.h"
#include "logger.h"


t_log* iniciar_logger(char*,t_log_level);
t_config* iniciar_config(char*);
char* preparar_ruta(char*,char*);
char* obtener_codigo_operacion(op_code);