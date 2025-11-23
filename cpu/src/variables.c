#include "variables.h"

t_log *logger = NULL;
t_log *logger_aux = NULL;
t_config_cpu *config_cpu = NULL;

bool flag_fin_programa = false;
bool flag_interrupt = false;
bool flag_desalojo = false;
bool flag_ejecutando = false;
t_motivo_desalojo motivo_desalojo = CONTINUE;
t_contexto_cpu *contexto = NULL;

int socket_server_interrupt = -1;
int socket_server_dispatch = -1;
int socket_memoria = -1;

pthread_t thr_server_interrupt;
pthread_t thr_server_dispatch;