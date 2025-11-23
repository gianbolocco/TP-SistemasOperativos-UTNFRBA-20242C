#ifndef OPERACIONES_MEMORIA_H_
#define OPERACIONES_MEMORIA_H_

// librerias c

// librerias catedra
#include <commons/string.h>

// librerias grupo
#include "estructuras.h"
#include "variables.h"
#include <utils/client.h>
#include <utils/comunications.h>
#include "utils.h"

int iniciar_conexion_memoria();
void finalizar_conexion_memoria(int socket_cliente);
op_code crear_proceso_en_memoria(t_tcb *tcb_0, int socket_cliente);
op_code finalizar_proceso_en_memoria(t_pcb *proceso, int socket_cliente);
op_code crear_hilo_en_memoria(t_tcb *hilo, int socket_cliente);
op_code finalizar_hilo_en_memoria(t_tcb *hilo, int socket_cliente);
op_code ejecutar_dump_memory(t_tcb *hilo, int socket_cliente);

#endif /* OPERACIONES_MEMORIA_H_ */