#ifndef OPERACIONES_CPU_H_
#define OPERACIONES_CPU_H_

// librerias c
#include <commons/string.h>

// librerias catedra

// librearias grupo
#include "estructuras.h"
#include "variables.h"
#include <utils/comunications.h>
#include <utils/client.h>

void iniciar_conexion_cpu_dispath();
void iniciar_conexion_cpu_interrupt();
void finalizar_conexion_cpu();
void enviar_hilo_a_cpu();
void enviar_desalojo_a_cpu();

#endif /* OPERACIONES_CPU_H_ */