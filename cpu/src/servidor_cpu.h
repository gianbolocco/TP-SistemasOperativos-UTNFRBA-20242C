#ifndef SERVIDOR_CPU_H_
#define SERVIDOR_CPU_H_

// librerias de c
#include <pthread.h>

// librerias catedra

// librerias cpu
#include "estructuras.h"
#include "variables.h"
#include <utils/server.h>
#include "ciclo_instruccion.h"
#include "operaciones_memoria.h"
#include "operaciones_kernel.h"
#include <utils/paquete.h>
#include <utils/estructuras_globales.h>
#include <utils/comunications.h>

void iniciar_servidor_cpu();
void finalizar_servidor_cpu();
void *comenzar_servidor_dispatch();
void *comenzar_servidor_interrupt();

#endif /* SERVIDOR_CPU_H_ */