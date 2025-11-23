#ifndef PLANIFICADOR_CORTO_PLAZO_H_
#define PLANIFICADOR_CORTO_PLAZO_H_

// librerias c
#include <stdbool.h>
#include <pthread.h>

// librerias catedra

// librearias grupo
#include "estructuras.h"
#include "variables.h"
#include "algoritmos_planificacion.h"
#include "syscalls.h"
#include "utils.h"

void inicializar_planificador_corto_plazo();
void finalizar_planificador_corto_plazo();
void *ejecutar_planificador_corto_plazo();
void ejecutar_hilo(bool first_run);

#endif /* PLANIFICADOR_CORTO_PLAZO_H_ */