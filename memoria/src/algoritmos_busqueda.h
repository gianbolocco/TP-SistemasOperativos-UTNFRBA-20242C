#ifndef ALGORITMOS_BUSQUEDA_H_
#define ALGORITMOS_BUSQUEDA_H_

//Librerias memoria
#include "estructuras.h"
#include "variables.h"

// Función que implementa el algoritmo de First Fit
t_particion* first_fit(uint32_t tamanio_proceso);

// Función que implementa el algoritmo de Best Fit
t_particion* best_fit(uint32_t tamanio_proceso);

// Función que implementa el algoritmo de Worst Fit
t_particion* worst_fit(uint32_t tamanio_proceso);

#endif /*ALGORITMOS_H_*/