// replicar logica en filesystem y en memo
// en cpu sigue casi igual solo que voy a tener una cocnexion que es la del kernel
// estoy haciendo la base del grafico
// kernel se conecta dos veces, una como despacher y la otra el que pasa la info para procesar (almacenar dos sockets)

#include "servidor_memoria.h"

t_client_connection *connection_info = NULL;

void iniciar_servidor_memoria()
{
    // Se castea la variable listen_port a void * debido a que el hilo accede a una direccion de memoria
    // es decir, desconoce cual es el tipo
    pthread_create(&thr_server, 0, comenzar_servidor_escucha, NULL);
    // NO QUIERO PASAR NINGUNA DIRECCION DE MEMORIA
}

void *comenzar_servidor_escucha()
{
    server_socket = server_start(config_datos->puerto_escucha, logger_aux);
    if (server_socket == -1)
    { // hace la escucha del puerto
        aceptar_clientes = false;
        flag_fin_programa = true;
        pthread_exit(0);
    }
    while (aceptar_clientes)
    {
        connection_info = (t_client_connection *)malloc(sizeof(t_client_connection));
        connection_info->socket = client_wait(server_socket, logger_aux);
        if (connection_info->socket != -1)
        { // Me presento, yo soy memoria y me dice quien es el cliente
            connection_info->HS_module = hs_server_to_module_get_type(connection_info->socket, HSMEMORIA, logger_aux);
            // crea el hilo para procesar esa conexion
            pthread_create(&thr_server_conn, 0, procesar_cliente, (void *)connection_info);
            // este me permite tener multiples conex para el mismo revidor, proceso a los clientes en paralelo, cada uno tiene su hilo
        }
        else
        {
            free(connection_info);
        }

        // log conexion
    }
    pthread_exit(0);
}

void finalizar_servidor_memoria()
{
    // me mata
    aceptar_clientes = false;
    pthread_join(thr_server_conn, NULL);
    pthread_join(thr_server, NULL);
    if (server_socket != -1)
        socket_destroy(server_socket);
    log_info(logger_aux, "Thread Memoria Server: finalizado");
}

void *procesar_cliente(void *ptr)
{
    // ejemplo para saber q funciones tipo hace cada modulo conmigo
    if (!ptr)
        pthread_exit(0);
    t_client_connection *conn = (t_client_connection *)ptr;
    switch (conn->HS_module)
    {
    case HSKERNEL: // hand shake que estan en un enum (work)
        log_info(logger_aux, "Thread Memoria Client: Se conecto el modulo Kernel");
        log_info(logger, "## Kernel Conectado - FD del socket: %d", conn->socket);
        procesar_peticion_kernel(conn->socket);
        log_info(logger_aux, "Thread Memoria Client: Se desconectara el modulo Kernel");
        break;
    case HSCPU:
        log_info(logger_aux, "Thread Memoria Client: Se conecto el modulo CPU");
        procesar_peticion_cpu(conn->socket);
        log_info(logger_aux, "Thread Memoria Client: Se desconectara el modulo CPU");
        break;
    default:
        log_warning(logger_aux, "Thread Memoria Client: Cliente desconocido");
        break;
    }
    socket_destroy(conn->socket);
    free(conn);
    pthread_exit(0);
}
