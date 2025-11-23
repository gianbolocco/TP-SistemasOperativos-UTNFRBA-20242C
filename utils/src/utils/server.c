#include "server.h"

int server_start(char *port, t_log *logger) {
    int server_socket;
    struct addrinfo hints, *servinfo;
    
    // Configuro los hints para que el socket use IPv4?, sea de tipo TCP y se asosie auto a la IP del servidor
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // Asignar la dirección IP del servidor auto

    // Obtengo la información de la dirección del servidor
    if (getaddrinfo(NULL, port, &hints, &servinfo) != 0) {
        log_error(logger, "Error en getaddrinfo");
        return -1;
    }

    // Creo el socket del servidor
    server_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (server_socket == -1) {
        log_error(logger, "Error al crear el socket");
        freeaddrinfo(servinfo);
        return -1;
    }

    // Configuro el socket para que se pueda reutilizar la dirección inmediatamente después de cerrarse
    int enable = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        log_error(logger, "Error en setsockopt");
        freeaddrinfo(servinfo);
        close(server_socket);
        return -1;
    }

    // Asocio el socket a un puerto específico
    if (bind(server_socket, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        log_error(logger, "Error al hacer bind");
        freeaddrinfo(servinfo);
        close(server_socket);
        return -1;
    }

    freeaddrinfo(servinfo);

    // Pongo el socket en modo de escucha para aceptar conexiones entrantes
    if (listen(server_socket, SOMAXCONN) < 0) {
        log_error(logger, "Error al poner a escuchar el socket");
        close(server_socket);
        return -1;
    }

    log_info(logger, "Servidor escuchando en el puerto: %s", port);
    return server_socket;
}

int client_wait(int server_socket, t_log *logger) {
    int client_socket;
    
    // Intento aceptar un nuevo cliente con un timeout de 2 segundos
    for (int i = 0; i < 2; i++) {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket >= 0) {
            break;  // Si se acepta un cliente, salgo del bucle
        }
        sleep(1);  // Espero un segundo antes de intentar de nuevo
    }
    
    if (client_socket < 0) {
        log_error(logger, "Error al aceptar un cliente");
        return -1;
    }
    
    return client_socket;
}

int process_create(int pid, char *pseudo_code_file, int process_size, t_log *logger) {
    log_info(logger, "Creando proceso PID: %d con archivo: %s y tamaño: %d", pid, pseudo_code_file, process_size);
    // lógica para crear un proceso, incluyendo la reserva de memoria y el registro en el PCB
    // Simulamos la creación del proceso en el log por ahora :)
    return 0;
}

int thread_create(int pid, int tid, int priority, char *pseudo_code_file, t_log *logger) {
    log_info(logger, "Creando hilo TID: %d para proceso PID: %d con prioridad: %d", tid, pid, priority);
    // Gonza crear un hilo, incluyendo la reserva de recursos y el registro en el TCB
    // idem
    return 0;
}

int process_terminate(int pid, t_log *logger) {
    log_info(logger, "Terminando proceso PID: %d", pid);
    // terminar un proceso, liberando memoria y eliminando el PCB
    // idem
    return 0;
}

int thread_terminate(int pid, int tid, t_log *logger) {
    log_info(logger, "Terminando hilo TID: %d para proceso PID: %d", tid, pid);
    // terminar un hilo, liberando recursos y eliminando el TCB
    // idem
    return 0;
}

int connect_to_memory(char *ip, char *port, t_log *logger) {
    log_info(logger, "Conectando al módulo Memoria en %s:%s", ip, port);
    // conectar al módulo de Memoria
    // idem
    return 0;
}

int connect_to_cpu(char *ip, char *port, t_log *logger) {
    log_info(logger, "Conectando al módulo CPU en %s:%s", ip, port);
    // conectar al módulo de CPU
    // idem
    return 0;
}
