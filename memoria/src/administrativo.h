#ifndef ADMINISTRATIVO_H_
#define ADMINISTRATIVO_H_

//librerias de c
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//librerias catedra 
#include <commons/string.h>

//librerias memoria
#include "estructuras.h"
#include "variables.h"
#include "algoritmos_busqueda.h"
#include "algoritmos_particiones.h"
#include "./utils/estructuras_globales.h"


void inicializar_estructuras_memoria(); //
void eliminar_estructuras_memoria();//memory leacks
// Función que verifica si hay espacio en memoria para el proceso
op_code entra_en_memoria(uint32_t tamanio_proceso);

bool ordenar_particiones_por_inicio(void* a, void* b);

// Función para asignar espacio en memoria al proceso
t_particion* asignar_particion(uint32_t tamanio_proceso);

t_list *buscar_pcb(int pid);
t_pcb_memoria* buscar_pcb_tid(int pid, int tid);
t_pcb_memoria *buscar_pcb_tid_por_particion(t_particion *particion);
// Función para liberar un proceso y su espacio en memoria

void liberar_proceso(t_pcb_memoria* pcb);
void liberar_hilo(t_pcb_memoria *pcb);

#endif /*ADMINISTRATIVO_H_*/