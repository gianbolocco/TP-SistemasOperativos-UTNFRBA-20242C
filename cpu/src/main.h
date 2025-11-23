#ifndef MAIN_H_
#define MAIN_H_

// librerias de c
#include <pthread.h>
#include <stdbool.h>

// librerias catedra
#include <commons/log.h>

// librerias cpu
#include "estructuras.h"
#include "variables.h"
#include "servidor_cpu.h"
#include "operaciones_memoria.h"
#include "config_cpu.h"

int main();
void sigintHandler(int signum);

#endif /* MAIN_H_ */