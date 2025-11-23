#ifndef CONFIG_KERNEL_H_
#define CONFIG_KERNEL_H_

// librerias c
#include <stdlib.h>

// librerias catedra
#include <commons/config.h>
#include <commons/string.h>

// librearias grupo
#include "estructuras.h"
#include "variables.h"

void leer_configuracion();
void destroy_configuracion();

#endif /* CONFIG_KERNEL_H_ */