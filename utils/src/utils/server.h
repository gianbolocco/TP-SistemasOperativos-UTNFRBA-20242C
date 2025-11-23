#ifndef SERVER_H_
#define SERVER_H_
/* iniciar el servidor y esperar conexiones de clientes. El TP nuevo pide q el servidor tmb debería manejar la planificación de procesos, hilos, y maybe la comunicación de memo o cpu. Asi que estas ultimas las defino en cada uno, comunicaciones .h */

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#include <commons/log.h>
#include <commons/collections/list.h>

#include <string.h>
#include <assert.h>

#include "comunications.h"

int server_start(char *, t_log *);
int client_wait(int, t_log *);




#endif /* SERVER_H_ */