#ifndef SRC_ARCHIVO_DUMP_
#define SRC_ARCHIVO_DUMP_

#include "esquemas.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "config.h"
#include "math.h"
#include <commons/memory.h>
#include <utils/paquete.h>
#include <utils/comunications.h>

void crear_archivo_dump(char *, uint32_t, void *, int);
bool verificar_espacio_bitmap(uint32_t, char *);
t_list *reservar_bitmap(int, char *);
void crear_archivo_metadata(char *, uint32_t, uint32_t);
void actualizar_bloque_indices(t_list *, char *);
void actualizar_bloque_datos(t_list *, void *, uint32_t, char *);

void enviar_respuesta_peticion(uint32_t resultado, int socket);
#endif /* SRC_ARCHIVO_DUMP_ */
