#ifndef OPERACIONES_KERNEL_H_
#define OPERACIONES_KERNEL_H_

// librerias de c

// librerias catedra
#include <commons/string.h>

// librerias cpu
#include "estructuras.h"
#include "variables.h"
#include <utils/paquete.h>
#include <utils/client.h>
#include "utils.h"

void recibir_hilo_a_ejecutar(int socket_cliente);
void enviar_motivo_de_desalojo(int socket_cliente);


#endif /* OPERACIONES_KERNEL_H_ */