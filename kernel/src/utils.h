#ifndef UTILS_H_
#define UTILS_H_

// librerias c
#include <stdio.h>
#include <stdlib.h>

// librerias catedra
#include <commons/log.h>
#include <commons/string.h>
#include <commons/temporal.h>

// librearias grupo
#include "estructuras.h"
#include "variables.h"

t_pcb *crear_pcb(char *archivo, int tamanio, int prioridad);
t_tcb *crear_tcb(t_pcb *proceso, char *archivo, int prioridad);
t_recurso *crear_recurso(char *nombre, t_pcb *proceso);
void liberar_pcb(t_pcb *proceso);
void liberar_hilo(t_tcb *hilo);
void liberar_recurso(t_recurso *recurso);
t_pcb* buscar_proceso(uint32_t pid);
t_tcb* buscar_hilo(t_pcb* proceso, uint32_t param_tid);
t_recurso *buscar_recurso(char *nombre, t_pcb *proceso);
void encolar_en_new(t_pcb *proceso);
void encolar_en_ready(t_tcb *hilo);
void encolar_en_blocked(t_tcb *hilo, t_motivo_bloqueo motivo_bloqueo);
void desencolar_estado_actual(t_tcb *hilo);
char *obtener_estado_proceso(t_estado estado);
char *obtener_motivo_bloqueo(t_motivo_bloqueo motivo_bloqueo);
char *nombre_syscall(t_motivo_desalojo motivo);

#endif /* UTILS_H_ */