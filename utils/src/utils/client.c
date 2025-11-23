#include "client.h"

int create_connection(char *ip, char *port, t_log *logger) {
    struct addrinfo hints, *server_info;
    
    // Configuro los hints para IPv4 y TCP
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    
    // Obtengo la información de la dirección del servidor
    if (getaddrinfo(ip, port, &hints, &server_info) != 0) {
        log_error(logger, "Error en getaddrinfo");
        return -1;
    }
    
    // Creo el socket para conectar al servidor
    int client_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (client_socket == -1) {
        log_error(logger, "Error al crear el socket");
        freeaddrinfo(server_info);
        return -1;
    }
    
    // Conecto el socket al servidor
    if (connect(client_socket, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        log_error(logger, "Error al conectar al servidor");
        close(client_socket);
        freeaddrinfo(server_info);
        return -1;
    }
    
    log_info(logger, "Conexión exitosa al servidor en %s:%s", ip, port);
    freeaddrinfo(server_info);
    
    return client_socket;
}

int send_pseudo_code(int socket, char *pseudo_code, t_log *logger) {
    log_info(logger, "Enviando pseudocódigo al servidor");
    // enviar el pseudocódigo al servidor
    // simulo
    return 0;
}

int receive_execution_result(int socket, t_log *logger) {
    log_info(logger, "Recibiendo resultado de la ejecución");
    // recibir el resultado de la ejecución desde el servidor
    // simulo
    return 0;
}

int syscall_process_create(int socket, char *pseudo_code_file, int process_size, int priority, t_log *logger) {
    log_info(logger, "Syscall para crear proceso con archivo: %s", pseudo_code_file);
    // syscall que crea un proceso
    // simulo
    return 0;
}

int syscall_thread_create(int socket, int pid, int priority, t_log *logger) {
    log_info(logger, "Syscall para crear hilo para proceso PID: %d", pid);
    // syscall que crea un hilo
    // simulo
    return 0;
}

int syscall_process_terminate(int socket, int pid, t_log *logger) {
    log_info(logger, "Syscall para terminar proceso PID: %d", pid);
    // syscall que termina un proceso
    // simulo
    return 0;
}

int syscall_thread_terminate(int socket, int pid, int tid, t_log *logger) {
    log_info(logger, "Syscall para terminar hilo TID: %d para proceso PID: %d", tid, pid);
    // syscall que termina un hilo
    // simulo
    return 0;
}
