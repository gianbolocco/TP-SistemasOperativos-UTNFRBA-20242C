#ifndef OPERACIONES_MEMORIA_H
#define OPERACIONES_MEMORIA_H

// librerias de c
#include <stdio.h>
#include <stdbool.h>

// librerias catedra
#include <commons/string.h>

// librerias cpu
#include "estructuras.h"
#include "variables.h"
#include <utils/estructuras_globales.h>
#include <utils/client.h>
#include <utils/paquete.h>
#include "utils.h"
#include <utils/buffer.h>

void iniciar_conexion_memoria();
void finalizar_conexion_memoria();

void obtener_contexto();
void obtener_intruccion();
void actualizar_contexto();
void read_mem();
void write_mem();

#endif /* OPERACIONES_MEMORIA_H */