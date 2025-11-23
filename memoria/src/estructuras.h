#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

// librerias de c
#include <stdint.h>

// librerias catedra
#include <commons/collections/list.h>

typedef struct
{
    char *puerto_escucha;
    char *ip_filesystem;
    char *puerto_filesystem;
    int tamanio_memoria;
    char *path_instrucciones;
    int retardo_respuesta;
    char *esquema; // particiones fijas o dinamicas
    char *algoritmo_de_busqueda;
    char **particiones;
    char *log_level;
} t_config_memoria;

typedef struct
{
    int socket; // puerto de canal de comunicacion
    int HS_module;
} t_client_connection;

typedef struct
{
    uint32_t tamanio;
    uint32_t inicio;
    uint32_t fin;
    bool estaOcupado;
} t_particion;

typedef struct
{
    int pid; // cada una vale 32
    int tid;
    uint32_t AX;
    uint32_t BX;
    uint32_t CX;
    uint32_t DX;
    uint32_t EX;
    uint32_t FX;
    uint32_t GX;
    uint32_t HX;
    uint32_t PC;
    uint32_t tamanio;
    uint32_t base;
    uint32_t limite;       // todo estos 96 ul son los que voy a necesitar para comunicaciones
    t_list *instrucciones; // Lista de instrucciones
} t_pcb_memoria;

#endif /* ESTRUCTURAS_H_ */