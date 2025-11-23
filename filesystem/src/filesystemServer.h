#ifndef FILESYSTEM_SERVER_H_
#define FILESYSTEM_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/config.h>
#include <utils/server.h>
#include <utils/comunications.h>
#include "log.h"
#include "client_sockets.h"
#include "archivo_dump.h"
#include "config.h"

extern int server_socket;
extern bool accept_connections;
extern bool end_program_flag;

extern pthread_t thr_server;
extern pthread_t thr_server_conn;

void start_filesystem_server();
void *start_server_listen();
void end_filesystem_server();
void *process_client_entry(void *ptr);
void atender_memory_dump(int cliente);

#endif /* FILESYSTEM_SERVER_H_ */
