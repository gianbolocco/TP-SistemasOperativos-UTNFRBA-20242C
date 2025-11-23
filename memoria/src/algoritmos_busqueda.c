#include "algoritmos_busqueda.h"

// Algoritmo de First Fit
t_particion *first_fit(uint32_t tamanio_proceso)
{
    for (int i = 0; i < list_size(tablaParticiones); i++)
    {
        t_particion *particion = list_get(tablaParticiones, i);
        if (!particion->estaOcupado && particion->tamanio >= tamanio_proceso)
        {
            return particion; // Retornar partición adecuada
        }
    }
    log_warning(logger, "No se encontró partición adecuada con First Fit");
    return NULL; // No se encontró una partición adecuada
}

// Algoritmo de Best Fit
t_particion *best_fit(uint32_t tamanio_proceso)
{
    t_particion *mejor_particion = NULL;
    for (int i = 0; i < list_size(tablaParticiones); i++)
    {
        t_particion *particion = list_get(tablaParticiones, i);
        if (!particion->estaOcupado && particion->tamanio >= tamanio_proceso)
        {
            if (mejor_particion == NULL || particion->tamanio < mejor_particion->tamanio)
            {
                mejor_particion = particion;
            }
        }
    }
    if (mejor_particion == NULL)
    {
        log_warning(logger, "No se encontró partición adecuada con Best Fit");
    }
    return mejor_particion;
}

// Algoritmo de Worst Fit
t_particion *worst_fit(uint32_t tamanio_proceso)
{
    t_particion *peor_particion = NULL;
    for (int i = 0; i < list_size(tablaParticiones); i++)
    {
        t_particion *particion = list_get(tablaParticiones, i);
        if (!particion->estaOcupado && particion->tamanio >= tamanio_proceso)
        {
            if (peor_particion == NULL || particion->tamanio > peor_particion->tamanio)
            {
                peor_particion = particion;
            }
        }
    }
    if (peor_particion == NULL)
    {
        log_warning(logger, "No se encontró partición adecuada con Worst Fit");
    }
    return peor_particion;
}
