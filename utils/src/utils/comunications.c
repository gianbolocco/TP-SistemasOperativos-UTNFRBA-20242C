#include "comunications.h"
/* HANDSHAKE  wiii de mis comunicatio.h que despues uso en TODOS los modulos server y cliente*/
#define HANDSHAKE 1

bool hs_server_to_module_valid(int socket_client, hs_code hs_module_origin, t_log *logger)
{
    int HS_Recv_A = -1;
    int HS_Recv_B = -1;
    int HS_Origin = hs_module_origin;
    // Recibo conexion
    recv(socket_client, &HS_Recv_A, sizeof(int), MSG_WAITALL);
    // Mando quien soy
    send(socket_client, &HS_Origin, sizeof(int), 0);
    // Espero confirmacion
    recv(socket_client, &HS_Recv_B, sizeof(int), MSG_WAITALL);
    // Chequeo si es el modulo se desconecta
    if (HS_Recv_B == HSOK)
    {
        log_info(logger, "Handshake valido - Conexion aceptada");
        return true;
    }
    else
    {
        log_warning(logger, "Handshake invalido - Conexion rechazada");
        return false;
    }
}

int hs_server_to_module_get_type(int socket_client, hs_code hs_module_origin, t_log *logger)
{
    int HS_Recv_A = -1;
    int HS_Recv_B = -1;
    int HS_Origin = hs_module_origin;
    // Recibo conexion
    recv(socket_client, &HS_Recv_A, sizeof(int), MSG_WAITALL);
    // Mando quien soy
    send(socket_client, &HS_Origin, sizeof(int), 0);
    // Espero confirmacion
    recv(socket_client, &HS_Recv_B, sizeof(int), MSG_WAITALL);
    // Chequeo si es el modulo se desconecta
    if (HS_Recv_B == HSOK)
    {
        log_info(logger, "Handshake valido - Conexion aceptada");
        return HS_Recv_A;
    }
    else
    {
        log_warning(logger, "Handshake invalido - Conexion rechazada");
        return -1;
    }
}

bool hs_client_to_module_valid(int socket_server, hs_code hs_module_origin, hs_code hs_module_expected, t_log *logger)
{
    int HS_Recv_A = -1;
    int HS_Send_A = -1;
    int HS_Origin = hs_module_origin;
    int HS_Expected = hs_module_expected;
    // Mando quien soy
    send(socket_server, &HS_Origin, sizeof(int), 0);
    // Recibo quien es el otro
    recv(socket_server, &HS_Recv_A, sizeof(int), MSG_WAITALL);
    // Chequeo si es el modulo que espero
    if (HS_Recv_A == HS_Expected)
    {
        log_info(logger, "Handshake valido - Conexion aceptada");
        HS_Send_A = HSOK;
    }
    else
    {
        log_warning(logger, "Handshake invalido - Conexion rechazada");
        HS_Send_A = HSFAIL;
    }
    send(socket_server, &HS_Send_A, sizeof(int), 0);
    return (HS_Send_A == HSOK);
}

/* Cliente - Servidor */

void socket_destroy(int socket)
{
    close(socket);
}

void enviar_mensaje(char *mensaje, uint8_t codigo_operacion, int socket_conexion) {
	t_package *paquete = crear_paquete(codigo_operacion, buffer_vacio());

	agregar_a_paquete(paquete, mensaje, strlen(mensaje) + 1);
    enviar_paquete(paquete, socket_conexion);
	eliminar_paquete(paquete);
}

void enviar_paquete(t_package *paquete, int socket_conexion) {
	//t_buffer *a_enviar = serializar_paquete(paquete);

	//send(socket_conexion, a_enviar->stream, a_enviar->size, 0);

    void *a_enviar = malloc(paquete->buffer->size + sizeof(uint32_t) + sizeof(uint8_t));
    int offset = 0;
    memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    if (send(socket_conexion, a_enviar, paquete->buffer->size + sizeof(uint32_t) + sizeof(uint8_t), MSG_NOSIGNAL) == -1)
    {
        log_error(logger, "Error al enviar el paquete");
        free(a_enviar);
        return;
    }
    free(a_enviar);
    return;

}

t_package *recibir_paquete(int socket_cliente) {
/*
	uint32_t cod_op = recibir_operacion(socket_cliente);
	if(cod_op == HSFAIL)
		return crear_paquete(cod_op, buffer_vacio());
	t_buffer *buffer = recibir_buffer(socket_cliente);
	t_package *paquete = crear_paquete(cod_op, buffer);

	return paquete;
*/
    t_package *paquete = malloc(sizeof(t_package));
    paquete->buffer = malloc(sizeof(t_buffer));

    // Primero recibimos el codigo de operacion
    recv(socket_cliente, &(paquete->codigo_operacion), sizeof(uint8_t), 0);
    // log_debug(logger, "Recibido codigo de operacion: %d", paquete->codigo_operacion);
    // Después ya podemos recibir el buffer. Primero su tamaño seguido del contenido
    recv(socket_cliente, &(paquete->buffer->size), sizeof(uint32_t), 0);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    if (paquete->buffer->size > 0)
        recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size, 0);

    return paquete;
}

uint8_t recibir_operacion(int socket_cliente) {
	uint8_t codigo_operacion;

	if(recv(socket_cliente, &codigo_operacion, sizeof(uint8_t), 0) <= 0) {
		return HSFAIL;
	}

	return codigo_operacion;
}

t_buffer *recibir_buffer(int socket_cliente) {
	t_buffer *buffer = buffer_vacio();

	recv(socket_cliente, &(buffer->size), sizeof(uint32_t), 0);
	buffer->stream = malloc(buffer->size);
	recv(socket_cliente, buffer->stream, buffer->size, 0);
	return buffer;
}
