#ifndef SRC_UTILS_PAQUETE_H_
#define SRC_UTILS_PAQUETE_H_

#include "estructuras_globales.h"
#include "buffer.h"

t_package *crear_paquete(uint8_t codigo_operacion, t_buffer *buffer);
void agregar_a_paquete(t_package *paquete, void *dato, uint32_t tamanio);
void eliminar_paquete(t_package *paquete);
t_buffer *serializar_paquete(t_package *paquete);
t_list *deserealizar_paquete(t_package *paquete);
char *deserealizar_mensaje(t_package *paquete);

#endif /* SRC_UTILS_PAQUETE_H_ */
