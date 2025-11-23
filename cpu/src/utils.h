#ifndef UTILS_H
#define UTILS_H

// librerias de c
#include <stdint.h>
#include <string.h>

// librerias catedra
#include <commons/string.h>

// librerias cpu
#include "estructuras.h"
#include "variables.h"

void set_registry(char *registro, uint32_t value);
uint32_t get_registry(char *registro);
void decode_instruccion_tipo();
void check_desalojo(t_motivo_desalojo nuevo_motivo);
void initialize_contexto();
void clean_contexto();
void destroy_contexto();
char *motivo_desalojo_to_string(t_motivo_desalojo motivo);

#endif /* UTILS_H */