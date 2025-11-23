#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

// librerias de c
#include <stdint.h>

// librerias catedra

// librerias cpu

typedef struct
{
    char *ip_memoria;
    char *puerto_memoria;
    char *puerto_dispatch;
    char *puerto_interrupt;
    char *log_level;
} t_config_cpu;

typedef enum
{
	SET,
	INSTR_READ_MEM,
	INSTR_WRITE_MEM,
	SUM,
	SUB,
	JNZ,
	LOG,
	THREAD_EXIT,
	PROCESS_EXIT,
	DUMP_MEMORY,
	INSTR_IO,
	PROCESS_CREATE,
	THREAD_CREATE,
	INSTR_THREAD_JOIN,
	THREAD_CANCEL,
	MUTEX_CREATE,
	MUTEX_LOCK,
	MUTEX_UNLOCK,
	UNKNOWN_INSTRUCTION,
} t_instruccion;

typedef struct
{
    uint32_t pid;
    uint32_t tid;
    uint32_t pc;
    uint32_t ax;
    uint32_t bx;
    uint32_t cx;
    uint32_t dx;
    uint32_t ex;
    uint32_t fx;
    uint32_t gx;
    uint32_t hx;
    uint32_t base;
    uint32_t limite;
    uint32_t direccion_fisica;
    t_instruccion instruccion_tipo;
    char *instruccion;
    char **instruccion_parametros;
} t_contexto_cpu;

#endif /* ESTRUCTURAS_H_ */