#ifndef MAIN_H_
#define MAIN_H_

//librerias de c
#include <signal.h>

//librerias catedra 
#include <commons/log.h>
#include <commons/config.h>

//librerias memoria
#include "variables.h"
#include "config_memoria.h"
#include "servidor_memoria.h"
#include "estructuras.h"
#include "administrativo.h"

//librerias grupo
#include <utils/utils.h>

int main(void);
void sigintHandler(int sig_num);
void finalizar_programa();


#endif /* MAIN_H_ */
