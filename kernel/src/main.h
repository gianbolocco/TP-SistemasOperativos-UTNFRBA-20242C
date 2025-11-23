#ifndef MAIN_H_
#define MAIN_H_

// librerias c

// librerias catedra
#include <commons/log.h>

// librearias grupo
#include "estructuras.h"
#include "variables.h"
#include "config_kernel.h"
#include "kernel.h"

int main(int argc, char *argv[]);
void sigintHandler(int signum);
void finalizar_programa();

#endif /* MAIN_H_ */