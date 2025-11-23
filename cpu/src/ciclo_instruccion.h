#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

// librerias de c

// librerias catedra

// librerias cpu
#include "estructuras.h"
#include "variables.h"
#include "instrucciones.h"
#include "operaciones_kernel.h"
#include "operaciones_memoria.h"
#include "utils.h"

void fetch();
void decode();
void execute();

void ejecutar_hilo();

#endif /* CICLO_INSTRUCCION_H_ */