#include "variables.h"

t_log *logger = NULL;
t_log *logger_aux = NULL;
t_config_kernel *config_kernel = NULL;
t_config *config = NULL;

t_list *cola_new = NULL;
t_list *cola_ready = NULL;
t_tcb *hilo_en_exec = NULL;
t_list *cola_blocked = NULL;

t_list *procesos_en_sistema = NULL;

sem_t hay_pcbs_en_new;
sem_t hay_pcbs_en_ready;
sem_t hay_pcbs_en_blocked;

pthread_mutex_t mutex_new;
pthread_mutex_t mutex_ready;
pthread_mutex_t mutex_blocked;

pthread_mutex_t mutex_io;



int pid_counter = 0;

int socket_cpu_dispatch = -1;
int socket_cpu_interrupt = -1;

bool largo_plazo = true;
bool flag_ejecutando_en_cpu = false;
bool flag_desalojar = false;

bool flag_fin_de_programa = false;
bool flag_planificador_corto_plazo_finalizado = false;
bool flag_planificador_largo_plazo_finalizado = false;

pthread_t thr_corto_plazo;
pthread_t thr_largo_plazo;
pthread_t hilo_quantum;
