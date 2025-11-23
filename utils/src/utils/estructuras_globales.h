#ifndef ESTRUCTURAS_GLOBALES_H
#define ESTRUCTURAS_GLOBALES_H

#include <stdint.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>

typedef enum
{
    SUCCESS,
    MENSAJE,
    PAQUETE,
    EJECUCION_HILO,
    INSTRUCCIONES,     // Pseudocodigo
    PCONTEXTO,         // Contexto de proceso
    PCONTEXTODESALOJO, // Contexto de proceso desalojo
    FIN_DE_CLOCK,
    INSTRUCCION,     // Instruccion
    INFO,            // Datos void*
    CREAR_PROCESO,
    FIN_PROCESO,
    CREAR_HILO,
    FIN_HILO,
    MEMORY_DUMP,
    OBTENER_CONTEXTO,
    ACTUALIZAR_CONTEXTO,
    OBTENER_INSTRUCCION,
    READ_MEM,
    WRITE_MEM,
    MAX_SIZE_ERROR,
    SIZE_ERROR,
    PATH_ERROR,
    MOTIVO_DESALOJO,
    TID_NOT_FOUND,
    PID_NOT_FOUND,
    INSTRUCCION_NOT_FOUND
} op_code;

typedef enum
{
    HSKERNEL,
    HSCPU,
    HSCPUINTERRUPTION,
    HSFS,
    HSMEMORIA,
    HSOK,
    HSFAIL
} hs_code;

typedef enum
{
    THREAD_JOIN,
    MUTEX,
    IO,
    M_DUMP,
    NONE
} t_motivo_bloqueo;

typedef struct
{
    uint32_t size;
    void *stream;
} t_buffer;

typedef struct
{
    uint8_t codigo_operacion;
    t_buffer *buffer;
} t_package;

typedef enum
{
    CONTINUE,
    M_DUMP_MEMORY,
    M_IO,
    M_PROCESS_CREATE,
    M_THREAD_CREATE,
    M_THREAD_JOIN,
    M_THREAD_CANCEL,
    M_MUTEX_CREATE,
    M_MUTEX_LOCK,
    M_MUTEX_UNLOCK,
    M_THREAD_EXIT,
    M_PROCESS_EXIT,

    INTERRUPCION, // en nuestro TP, solo entra fin de clock del algoritmo rr en esta categoria
    SEGFAULT
} t_motivo_desalojo;

#endif // ESTRUCTURAS_GLOBALES_H
