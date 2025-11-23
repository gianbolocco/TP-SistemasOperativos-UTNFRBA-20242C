#ifndef SERVIDOR_MEMORIA_H_
#define SERVIDOR_MEMORIA_H_

//librerias de c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//librerias catedra 
#include <commons/log.h>
#include <commons/config.h>

//librerias grupo
#include <utils/server.h>
#include <utils/comunications.h>

//librerias memoria
#include "variables.h"
#include "operaciones_kernel.h"
#include "operaciones_cpu.h"

void iniciar_servidor_memoria();

void *comenzar_servidor_escucha();

void finalizar_servidor_memoria();

void *procesar_cliente(void *ptr);



#endif /* SERVIDOR_MEMORIA_H_ */
