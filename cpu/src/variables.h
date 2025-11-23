#ifndef VARIABLES_H_
#define VARIABLES_H_

// librerias de c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// librerias catedra
#include <commons/config.h>
#include <commons/log.h>

// librerias cpu
#include "estructuras.h"
#include <utils/estructuras_globales.h>

extern t_log *logger;
extern t_log *logger_aux;
extern t_config_cpu *config_cpu;

extern bool flag_fin_programa;
extern bool flag_interrupt; // flag para saber si se recibio una interrupcion
extern bool flag_desalojo; // flag para saber si se debe desalojar el hilo actual
extern bool flag_ejecutando; // flag para saber si se esta ejecutando un hilo
extern t_motivo_desalojo motivo_desalojo; // motivo de desalojo del hilo actual
extern t_contexto_cpu *contexto; // contexto de ejecucion del hilo actual

extern int socket_server_interrupt; // socket del servidor de interrupciones
extern int socket_server_dispatch; // socket del servidor de dispatch
extern int socket_memoria; // socket de la memoria

extern pthread_t thr_server_interrupt;
extern pthread_t thr_server_dispatch;

#endif /* VARIABLES_H_ */