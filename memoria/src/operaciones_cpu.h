#ifndef OPERACIONES_CPU_H_
#define OPERACIONES_CPU_H_

//librerias de la catedra
#include <commons/string.h>

//librerias grupo
#include <utils/comunications.h>
#include <utils/utils.h>

//librerias memoria
#include "variables.h"
#include "estructuras.h"
#include "administrativo.h"

void procesar_peticion_cpu(int socket_cliente);

void obtener_contexto(t_package* paquete, int socket_cliente);

void actualizar_contexto(t_package* paquete, int socket_cliente);

void obtener_instruccion(t_package* paquete, int socket_cliente);

void read_mem(t_package* paquete, int socket_cliente);

void write_mem(t_package* paquete, int socket_cliente);

#endif /*OPERACIONES_CPU_H_*/