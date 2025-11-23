#ifndef SRC_UTILS_BUFFER_H_
#define SRC_UTILS_BUFFER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estructuras_globales.h"


t_buffer *buffer_vacio(void);
t_buffer *crear_buffer(void *stream, uint32_t size);
void agregar_a_buffer(t_buffer *buffer, void *src, uint32_t size);
void quitar_de_buffer(t_buffer *buffer, void *dest, uint32_t size);
void eliminar_buffer(t_buffer *buffer);

#endif /* SRC_UTILS_BUFFER_H_ */
