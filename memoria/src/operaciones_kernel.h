#ifndef OPERACIONES_KERNEL_H_
#define OPERACIONES_KERNEL_H_

// librerias de la catedra
#include <commons/string.h>
#include <commons/memory.h>
#include <commons/temporal.h>

// librerias grupo
#include <utils/comunications.h>
#include <utils/client.h>
#include <utils/utils.h>

// librerias memoria
#include "variables.h"
#include "estructuras.h"
#include "administrativo.h"

// switch que resive socket_cliente para saber de que canal de comunicacion tengo q leer
void procesar_peticion_kernel(int socket_cliente);

void creacion_de_proceso(t_package *paquete, int socket_cliente);

void finalizacion_de_proceso(t_package *paquete, int socket_cliente);

void creacion_de_hilo(t_package *paquete, int socket_cliente);

void finalizacion_de_hilo(t_package *paquete, int socket_cliente);

void memory_dump(t_package *paquete, int socket_cliente);

#endif /*OPERACIONES_KERNEL_H_*/