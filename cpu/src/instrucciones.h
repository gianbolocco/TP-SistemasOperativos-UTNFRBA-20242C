#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

// librerias de c
#include <stdint.h>
#include <stdbool.h>

// librerias catedra
#include <commons/string.h>

// librerias cpu
#include "estructuras.h"
#include "variables.h"
#include "utils.h"

void _set();
void _sum();
void _sub();
void _jnz();
void _log();
uint32_t mmu(char *registro);

#endif /* INSTRUCCIONES_H */