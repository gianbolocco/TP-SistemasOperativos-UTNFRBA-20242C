#ifndef VARIABLES_H_
#define VARIABLES_H_

//librerias de c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//librerias catedra 
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>

//librerias memoria
#include "estructuras.h"

extern t_log *logger;
extern t_log *logger_aux;
extern t_config_memoria* config_datos;
extern t_config *config;

extern bool flag_fin_programa;
extern int server_socket;
extern bool aceptar_clientes;

extern pthread_t thr_server;
extern pthread_t thr_server_conn;

extern t_list *tablaParticiones;
extern t_list *tablaPCBs;

extern void *memoria;
extern pthread_mutex_t memory_space_mutex;

#endif /* VARIABLES_H_ */