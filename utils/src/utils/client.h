#ifndef CLIENT_H_
#define CLIENT_H_
//Ahora el cliente tipo tiene que manejar tmb las syscalls y la interc de los modls

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>

#include <string.h>

#include <commons/log.h>

#include "comunications.h"

int create_connection(char* ip, char* puerto, t_log* logger);

/* 

Funciones del cliente

int create_connection(char *ip, char *port, t_log *logger);
int send_pseudo_code(int socket, char *pseudo_code, t_log *logger);
int receive_execution_result(int socket, t_log *logger);

Manejo de syscalls
PARTE DE OPERACIONES NO DEL SERVIDOR QUE COMINCA :( NO VA:
int syscall_process_create(int socket, char *pseudo_code_file, int process_size, int priority, t_log *logger);
int syscall_thread_create(int socket, int pid, int priority, t_log *logger);
int syscall_process_terminate(int socket, int pid, t_log *logger);
int syscall_thread_terminate(int socket, int pid, int tid, t_log *logger); */

#endif /* CLIENT_H_ */