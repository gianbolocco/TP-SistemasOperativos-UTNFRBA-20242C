#include "variables.h"


// --------------------------------------------
// Variables para el Servidor de Memoria :)
// --------------------------------------------

// Inicializo los loggers para registrar la actividad del servidor
t_log *logger = NULL;
t_log *logger_aux = NULL;
t_config *config = NULL;
t_config_memoria* config_datos = NULL;

// Variables para gestionar el flujo del programa
bool flag_fin_programa = false; // Indica si se debe finalizar el programa
int server_socket = -1; // Socket del servidor
bool aceptar_clientes = true; // Flag para aceptar nuevas conexiones

// Variables para manejar los hilos del servidor
pthread_t thr_server; // Hilo principal del servidor
pthread_t thr_server_conn; // Hilo para manejar conexiones individuales de clientes

t_list* tablaParticiones = NULL; // Lista para almacenar particiones
t_list* tablaPCBs = NULL; // Lista para almacenar PCBs

// Variables relacionadas con la gestión de la memoria
void* memoria = NULL;
pthread_mutex_t memory_space_mutex; // Mutex para asegurar acceso concurrente al espacio de memoria



