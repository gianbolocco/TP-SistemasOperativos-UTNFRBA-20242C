#include "paquete.h"

t_package *crear_paquete(uint8_t codigo_operacion, t_buffer *buffer) {
	t_package *paquete = (t_package *)malloc(sizeof(t_package));

	paquete->codigo_operacion = codigo_operacion;
	paquete->buffer = buffer;

	return paquete;
}

void agregar_a_paquete(t_package *paquete, void *dato, uint32_t tamanio) {
	agregar_a_buffer(paquete->buffer, &tamanio, sizeof(uint32_t)); // primero se agrega tamanio como payload
	agregar_a_buffer(paquete->buffer, dato, tamanio);
}

void eliminar_paquete(t_package *paquete) {
	eliminar_buffer(paquete->buffer);
	free(paquete);
}

t_buffer *serializar_paquete(t_package *paquete) {
    t_buffer *buffer = buffer_vacio();

    agregar_a_buffer(buffer, &(paquete->codigo_operacion), sizeof(op_code));
    agregar_a_buffer(buffer, &(paquete->buffer->size), sizeof(int));
    agregar_a_buffer(buffer, paquete->buffer->stream, paquete->buffer->size);

    return buffer;
}

t_list *deserealizar_paquete(t_package *paquete) {
	t_list *lista = list_create();
	t_buffer *buffer = paquete->buffer;

	while(buffer->size > 0) {
		uint32_t tamanio;
		quitar_de_buffer(buffer, &tamanio, sizeof(uint32_t));
		void *dato = malloc(tamanio);
		quitar_de_buffer(buffer, dato, tamanio);
		list_add(lista, dato);
	}

	return lista;
}

char *deserealizar_mensaje(t_package *paquete) {
	t_list *lista = deserealizar_paquete(paquete);
	char *mensaje = (char *)list_get(lista, 0);
	list_destroy(lista);

	return mensaje;
}
