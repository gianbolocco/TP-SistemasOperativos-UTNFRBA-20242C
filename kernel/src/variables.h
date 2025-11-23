#ifndef VARIABLES_H_
#define VARIABLES_H_

// librerias c
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>

// librerias catedra
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/config.h>

// librearias grupo
#include "estructuras.h"
#include <utils/estructuras_globales.h>

extern t_log* logger;
extern t_log* logger_aux;
extern t_config_kernel* config_kernel;
extern t_config *config;

extern t_list* cola_new; // Lista de t_tcb
extern t_list* cola_ready; // Lista de t_tcb
extern t_tcb* hilo_en_exec; // t_tcb
extern t_list* cola_blocked; // Lista de t_tcb

extern t_list* procesos_en_sistema; // Lista de t_pcb

extern sem_t hay_pcbs_en_new;
extern sem_t hay_pcbs_en_ready;
extern sem_t hay_pcbs_en_blocked;

extern pthread_mutex_t mutex_new;
extern pthread_mutex_t mutex_ready;
extern pthread_mutex_t mutex_blocked;

extern pthread_mutex_t mutex_io;

extern int pid_counter;

extern int socket_cpu_dispatch;
extern int socket_cpu_interrupt;

extern bool largo_plazo;
extern bool flag_ejecutando_en_cpu;
extern bool flag_desalojar;

extern bool flag_fin_de_programa;
extern bool flag_planificador_corto_plazo_finalizado;
extern bool flag_planificador_largo_plazo_finalizado;

extern pthread_t thr_corto_plazo;
extern pthread_t thr_largo_plazo;
extern pthread_t hilo_quantum;
#endif /* VARIABLES_H_ */