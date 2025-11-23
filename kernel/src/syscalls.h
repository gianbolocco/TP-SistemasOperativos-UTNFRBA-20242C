#ifndef SYSCALLS_H_
#define SYSCALLS_H_

// librerias c
#include <pthread.h>
#include <unistd.h>

// librerias catedra

// librearias grupo
#include "estructuras.h"
#include "variables.h"
#include "utils.h"
#include "operaciones_memoria.h"

void interrupcion();
void *dump_memory_handler(void *args);
void dump_memory();
void *io_handler(void *args);
void peticion_IO(int tiempo);
void iniciar_proceso(char *archivo, uint32_t prioridad, uint32_t tamanio);
void thread_create(char *archivo, uint32_t prioridad);
bool thread_join(uint32_t tid_bloqueador);
void thread_cancel(uint32_t tid_cancelado);
void mutex_create(char *recurso);
bool mutex_lock(char *recurso);
void mutex_unlock(char *recurso);
void thread_exit();
void process_exit();
void process_exit_thread(t_tcb *hilo);

#endif /* SYSCALLS_H_ */