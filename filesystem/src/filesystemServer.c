#include "filesystemServer.h"

int server_socket;
bool end_program_flag;
bool accept_connections = true;
pthread_t thr_server, thr_server_conn;

void start_filesystem_server()
{
    pthread_create(&thr_server, 0, start_server_listen, NULL);
    pthread_detach(thr_server);
}

void *start_server_listen()
{
    log_debug(logger_aux, "Puerto: %d", config->puerto_escucha);
    char* puerto = string_itoa(config->puerto_escucha);
    server_socket = server_start(puerto, logger_aux);
    if (server_socket == -1)
    {
        accept_connections = false;
        end_program_flag = true;
        pthread_exit(0);
    }

    while (accept_connections)
    {
        int socket_memoria = client_wait(server_socket, logger_aux);
        hs_server_to_module_valid(socket_memoria, HSFS, logger_aux);

        if (socket_memoria != -1)
        {
            int *socket_memoria_ptr = malloc(sizeof(int));
            *socket_memoria_ptr = socket_memoria;
            pthread_create(&thr_server_conn, 0, process_client_entry, (void *)socket_memoria_ptr);
            pthread_detach(thr_server_conn);
        }
    }
    pthread_exit(0);
    free(puerto);
}

void end_filesystem_server()
{
    accept_connections = false;
    if (server_socket != -1)
        socket_destroy(server_socket);
    pthread_mutex_lock(&mutex_auxLogger);
    log_info(logger_aux, "Thread Filesystem Server: finalizado");
    pthread_mutex_unlock(&mutex_auxLogger);
	log_destroy(logger_aux);
	log_destroy(logger);
	fclose(archivo_bitmap);
	fclose(archivo_bloques);
	munmap((void*)mappeo_bitmap, (int)ceil(config->block_count/8.0));
	munmap((void*)mappeo_bloques, config->block_count * config->block_size);
	destroy_config(config);
}

void *process_client_entry(void *socket)
{
    int socket_cliente = *(int *)socket;
    if (!socket)
    {
        pthread_exit(0);
    }

    pthread_mutex_lock(&mutex_auxLogger);
    log_info(logger_aux, "Thread Filesystem Client: Se conectó el módulo Memoria");
    pthread_mutex_unlock(&mutex_auxLogger);
    atender_memory_dump(socket_cliente);
    pthread_mutex_lock(&mutex_auxLogger);
    log_info(logger_aux, "Thread Filesystem Client: Se desconectará el módulo Memoria. socket numero %d", socket_cliente);
    pthread_mutex_unlock(&mutex_auxLogger);

    socket_destroy(socket_cliente);
    free(socket);

    pthread_exit(0);
}

void atender_memory_dump(int socket_cliente)
{
    t_package *paquete = recibir_paquete(socket_cliente);
    log_debug(logger_aux, "Recibi paquete!");
    if (paquete->codigo_operacion != MEMORY_DUMP)
    {
        pthread_mutex_lock(&mutex_auxLogger);
        log_error(logger_aux, "Error, FS solo acepta operacion: MEMORY_DUMP");
        pthread_mutex_unlock(&mutex_auxLogger);
        exit(EXIT_FAILURE);
    }

    t_list *datos = deserealizar_paquete(paquete);

    uint32_t tamanioArchivo = *((uint32_t *)list_get(datos, 0));
    char *nombreArchivo = (char *)list_get(datos, 1);
    //char *contenido = (char *)list_get(datos, 2);
    void *contenido = list_get(datos, 2);
    char *contenido_str = mem_hexstring(contenido, tamanioArchivo);
    log_debug(logger_aux, "Nombre: %s, tamanio: %d, contenido: %s", nombreArchivo, tamanioArchivo, contenido_str);
    crear_archivo_dump(nombreArchivo, tamanioArchivo, contenido, socket_cliente); // recibe: 1. tamanio del archivo, 2. nombre del archivo y 3.el contenido a grabar.
    eliminar_paquete(paquete);
    list_destroy_and_destroy_elements(datos, free);
    free(contenido_str);
    /*
    t_package *respuesta = crear_paquete(SUCCESS, buffer_vacio());
    enviar_paquete(respuesta, socket_cliente);
    eliminar_paquete(respuesta);
    */
}
