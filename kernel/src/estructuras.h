#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

// librerias c
#include <stdint.h>

// librerias catedra
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/temporal.h>

// librearias grupo
#include <utils/estructuras_globales.h>

typedef enum
{
    NEW,
    READY,
    BLOCKED,
    EXEC,
    EXIT
} t_estado;

typedef struct
{
    char *IP_MEMORIA;
    char *PUERTO_MEMORIA;
    char *IP_CPU;
    char *PUERTO_CPU_DISPATCH;
    char *PUERTO_CPU_INTERRUPT;
    char *ALGORITMO_PLANIFICACION;
    int QUANTUM;
    char *LOG_LEVEL;
} t_config_kernel;

typedef struct
{
    char *nombre;
    t_list *hilos; // Cola de t_tcb
    int owner;
} t_recurso;

typedef struct
{
    uint32_t pid;
    uint32_t tid;
    uint32_t prioridad;
    t_estado estado;
    char *archivo;
    t_temporal *temporal;
    uint32_t tid_bloqueador;
    t_motivo_bloqueo motivo_bloqueo;
} t_tcb;

typedef struct
{
    uint32_t pid;
    t_list *threads; // Lista de t_tcb
    t_list *mutex; // Lista de t_recurso

    t_estado estado;

    uint32_t tamanio;
    uint32_t tid_contador;
} t_pcb;

typedef struct
{
    t_tcb *hilo;
    int tiempo;
} t_io_params;

#endif /* ESTRUCTURAS_H_ */