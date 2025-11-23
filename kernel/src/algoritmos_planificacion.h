#ifndef ALGORITMOS_PLANIFICACION_H_
#define ALGORITMOS_PLANIFICACION_H_

// librerias c
#include <unistd.h>

// librerias catedra
#include <commons/temporal.h>
#include <commons/string.h>

// librearias grupo
#include "estructuras.h"
#include "variables.h"
#include "operaciones_cpu.h"

t_tcb *planificar_corto_plazo();

bool mayor_prioridad_nuevo(t_tcb *hilo, t_tcb *nuevo_pcb);

t_tcb *algoritmo_prioridades();
t_tcb *algoritmo_fifo();
void *rr_handler(void* args);

#endif /* ALGORITMOS_PLANIFICACION_H_ */
