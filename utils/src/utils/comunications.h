//aca hago mis enums y funciones para la comunicacion de datos entre modulos
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>     // send - recv
#include <unistd.h>         // close
#include <string.h>
#include <utils/logger.h>
#include <commons/log.h>
//#include "types.h"
#include "./estructuras_globales.h"
#include "paquete.h"

/* OPERACIONES DE ENVIO DE INFORMACION */



/* CLIENTE -> SERVIDOR -> PROGRAMA */
t_package *get_package(int socket, t_log *logger);

/* HANDSHAKE */

//aca marcos explico como funciona: marcos es un genioooo
/**
    * @NAME: hs_server_to_module_valid
    * @DESC: Funcion para recibir un handshake desde un cliente y validarlo
             En este caso no se controla en el servidor que al modulo que espero
             Al no contolarse, el cliente actua como Maestro y el servidor como Esclavo
             Se debe permitir al cliente que envie cualquier tipo de operacion
             Se procede con la primicia de que el cliente sabe lo que tiene que enviar
    * @PARAMS:
    *        socket_server - el socket del cliente
    *        hs_module_origin - el modulo que envia el handshake
    *        logger - el logger para escribir los mensajes
    * @RETURN:
    *        retorna true en caso de que el handshake sea valido, false en caso contrario
    */
bool hs_server_to_module_valid(int socket_client, hs_code hs_module_origin, t_log *logger);

int hs_server_to_module_get_type(int socket_client, hs_code hs_module_origin, t_log *logger);

/**
 * @NAME: hs_client_to_module_valid
 * @DESC: Funcion para enviar un handshake a un servidor y validar la conexion esperada a un modulo
 * @PARAMS:
 *        socket_server - el socket del servidor
 *        hs_module_origin - el modulo que envia el handshake
 *        hs_module_expected - el modulo que espera recibir
 *        logger - el logger para escribir los mensajes
 * @RETURN:
 *        retorna true en caso de que el handshake sea valido, false en caso contrario
 */
bool hs_client_to_module_valid(int socket_server, hs_code hs_module_origin, hs_code hs_module_expected, t_log *logger);

/* Cliente - Servidor */

void socket_destroy(int);

void enviar_mensaje(char *mensaje, uint8_t codigo_operacion, int socket_conexion);
void enviar_paquete(t_package *paquete, int socket_conexion);
t_package *recibir_paquete(int socket_cliente);
uint8_t recibir_operacion(int socket_cliente);
t_buffer *recibir_buffer(int socket_cliente);

#endif /* COMMUNICATION_H_ */
