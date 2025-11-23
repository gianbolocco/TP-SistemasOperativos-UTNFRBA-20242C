#include "administrativo.h"

// Inicializar las estructuras de memoria
void inicializar_estructuras_memoria()
{
    memoria = calloc(1, config_datos->tamanio_memoria);
    tablaParticiones = list_create();
    tablaPCBs = list_create();
    // capital y lower case osea aca ignoro masyuduclas
    if (string_equals_ignore_case(config_datos->esquema, "FIJAS"))
    {
        inicializar_particiones_fijas();
    }
    else if (string_equals_ignore_case(config_datos->esquema, "DINAMICAS"))
    {
        inicializar_particiones_dinamicas();
    }
    else
    {
        log_error(logger, "Mall no es ni fija ni dinamicaa!!!");
        return;
    }

    log_debug(logger, "Estructuras de memoria inicializadas correctamente!!");
}

void eliminar_estructuras_memoria()
{
    if (memoria != NULL)
    {
        free(memoria);
    }
    // los numeros no ocupan memoria
    if (tablaParticiones != NULL)
    {
        for (int i = 0; i < list_size(tablaParticiones); i++)
        {
            t_particion *particion = list_get(tablaParticiones, i);
            free(particion);
        }
        list_destroy(tablaParticiones);
    }
    if (tablaPCBs != NULL)
    {
        for (int i = 0; i < list_size(tablaPCBs); i++)
        {
            t_pcb_memoria *pcb = list_get(tablaPCBs, i);

            for (int j = 0; j < list_size(pcb->instrucciones); j++)
            {
                char *instruccion = list_get(pcb->instrucciones, j);
                free(instruccion);
            }
            list_destroy(pcb->instrucciones);
            free(pcb);
        }
        list_destroy(tablaPCBs);
    }
}

// Función que verifica si hay suficiente espacio en memoria
op_code entra_en_memoria(uint32_t tamanio_proceso)
{
    if (config_datos->tamanio_memoria < tamanio_proceso)
    {
        return MAX_SIZE_ERROR;
    }
    else
    {
        for (int i = 0; i < list_size(tablaParticiones); i++)
        {
            t_particion *particion = list_get(tablaParticiones, i);
            if (!particion->estaOcupado && particion->tamanio >= tamanio_proceso)
            {
                return SUCCESS; // Hay espacio suficiente wiiiii
            }
        }
        return SIZE_ERROR;
    }
}


// Función para ordenar la lista de particiones
bool ordenar_particiones_por_inicio(void* a, void* b)
{
    t_particion* particion1 = (t_particion*) a;
	t_particion* particion2 = (t_particion*) b;
    return particion1->inicio < particion2->inicio;
}

// Asignación de espacio en memoria según el algoritmo elegido
t_particion *asignar_particion(uint32_t tamanio_proceso)
{
    t_particion *particion = NULL;

    // Selección del algoritmo de asignación
    if (string_equals_ignore_case(config_datos->algoritmo_de_busqueda, "FIRST"))
    {
        particion = first_fit(tamanio_proceso);
    }
    else if (string_equals_ignore_case(config_datos->algoritmo_de_busqueda, "BEST"))
    {
        particion = best_fit(tamanio_proceso);
    }
    else if (string_equals_ignore_case(config_datos->algoritmo_de_busqueda, "WORST"))
    {
        particion = worst_fit(tamanio_proceso);
    }
    else
    {
        log_error(logger, "Algoritmo de asignación desconocido");
    }

    if (particion == NULL)
    {
        return NULL;
    }

    particion->estaOcupado = true;

    if (string_equals_ignore_case(config_datos->esquema, "DINAMICAS"))
    {
        // voy a dividir mi particion simpre que sea > estricto que el tamaño de mi proceso
        if (particion->tamanio > tamanio_proceso)
        {
            uint32_t tamanio_restante = particion->tamanio - tamanio_proceso;
            // ahora tengo que cambiar el tamanio de particion q le asigno a proceso, tengo que cambiar el fin
            particion->tamanio = tamanio_proceso;
            particion->fin = particion->inicio + tamanio_proceso - 1; // porque cuento el 0
            if (tamanio_proceso == 0)
            {
                particion->fin = particion->inicio;
            }
            // descues crear una nueva particion y asignarle ewl tamanio restante y su nuevo inciio y agregarl oa a la lista de particiones
            t_particion *nueva_particion = malloc(sizeof(t_particion));
            if (tamanio_proceso == 0)
            {
                nueva_particion->inicio = particion->inicio;
                nueva_particion->fin = nueva_particion->inicio + tamanio_restante;
            }
            else
            {
                nueva_particion->inicio = particion->fin + 1;
                nueva_particion->fin = nueva_particion->inicio + tamanio_restante - 1;
            }
            nueva_particion->tamanio = tamanio_restante;
            nueva_particion->estaOcupado = false;
            list_add(tablaParticiones, nueva_particion);
            // ordenamos la lista de particiones por inicio
            list_sort(tablaParticiones, ordenar_particiones_por_inicio);
        }
    }

    log_info(logger_aux, "Estado tabla de particiones:");
    for (int i = 0; i < list_size(tablaParticiones); i++)
    {
        t_particion *particion = list_get(tablaParticiones, i);
        t_pcb_memoria *pcb = buscar_pcb_tid_por_particion(particion);
        if (pcb == NULL)
        {
            log_debug(logger_aux, "Partición %d - Inicio: %d - Fin: %d - Tamaño: %d - Ocupado: %d", i, particion->inicio, particion->fin, particion->tamanio, particion->estaOcupado);
        }
        else
        {
            log_debug(logger_aux, "Partición %d - Inicio: %d - Fin: %d - Tamaño: %d - Ocupado: %d - PID: %d - TID: %d", i, particion->inicio, particion->fin, particion->tamanio, particion->estaOcupado, pcb->pid, pcb->tid);
        }
    }

    return particion; // Retorna la partición asignada
}

t_list *buscar_pcb(int pid)
{
    t_list *lista_pcb = list_create();
    for (int i = 0; i < list_size(tablaPCBs); i++)
    {
        t_pcb_memoria *pcb = list_get(tablaPCBs, i);
        if (pcb->pid == pid)
        {
            list_add(lista_pcb, pcb);
        }
    }
    if (list_size(lista_pcb) == 0)
    {
        list_destroy(lista_pcb);
        return NULL;
    }
    return lista_pcb;
}

t_pcb_memoria *buscar_pcb_tid(int pid, int tid)
{
    for (int i = 0; i < list_size(tablaPCBs); i++)
    {
        t_pcb_memoria *pcb = list_get(tablaPCBs, i);
        if (pcb->pid == pid && pcb->tid == tid)
        {
            return pcb;
        }
    }
    return NULL;
}


// Función para liberar la partición ocupada por el proceso en memoria
void liberar_proceso(t_pcb_memoria *pcb)
{
    for (int i = 0; i < list_size(tablaParticiones); i++)
    {
        t_particion *particion = list_get(tablaParticiones, i);
        if (particion->inicio == pcb->base && particion->fin == pcb->limite && particion->estaOcupado)
        {
            particion->estaOcupado = false; // Liberar la partición
            pcb->base = 0;
            pcb->limite = 0;
            log_info(logger_aux, "Partición liberada para el proceso %d", pcb->pid);
            /* COMPACTACION FAKE */
            if (i > 0 && string_equals_ignore_case(config_datos->esquema, "DINAMICAS"))
            {
                // ordenamos la lista de particiones por inicio
                list_sort(tablaParticiones, ordenar_particiones_por_inicio);
                // Entro sabiendo que soy la segunda particion como minimo
                t_particion *particion_anterior = list_get(tablaParticiones, i - 1);
                if (particion_anterior != NULL && !particion_anterior->estaOcupado)
                {
                    particion_anterior->fin = particion->fin;
                    particion_anterior->tamanio = particion_anterior->tamanio + particion->tamanio;
                    // chequeo si hay una particion siguiente y no esta ocupada
                    if (list_size(tablaParticiones) > i + 1)
                    {
                        t_particion *particion_siguiente = list_get(tablaParticiones, i + 1);
                        if (particion_siguiente != NULL && !particion_siguiente->estaOcupado)
                        {
                            particion_anterior->fin = particion_siguiente->fin;
                            particion_anterior->tamanio = particion_anterior->tamanio + particion_siguiente->tamanio;
                            list_remove(tablaParticiones, i + 1);
                            free(particion_siguiente);
                        }
                    }
                    list_remove(tablaParticiones, i);
                    free(particion);
                } else {
                    if (list_size(tablaParticiones) > i + 1)
                    {
                        t_particion *particion_siguiente = list_get(tablaParticiones, i + 1);
                        if (particion_siguiente != NULL && !particion_siguiente->estaOcupado)
                        {
                            particion->fin = particion_siguiente->fin;
                            particion->tamanio = particion->tamanio + particion_siguiente->tamanio;
                            list_remove(tablaParticiones, i + 1);
                            free(particion_siguiente);
                        }
                    }
                }
            }
            break; // Salir del bucle una vez liberada
        }
    }
    log_info(logger_aux, "Estado tabla de particiones:");
    for (int i = 0; i < list_size(tablaParticiones); i++)
    {
        t_particion *particion = list_get(tablaParticiones, i);
        t_pcb_memoria *pcb = buscar_pcb_tid_por_particion(particion);
        if (pcb == NULL)
        {
            log_debug(logger_aux, "Partición %d - Inicio: %d - Fin: %d - Tamaño: %d - Ocupado: %d", i, particion->inicio, particion->fin, particion->tamanio, particion->estaOcupado);
        }
        else
        {
            log_debug(logger_aux, "Partición %d - Inicio: %d - Fin: %d - Tamaño: %d - Ocupado: %d - PID: %d - TID: %d", i, particion->inicio, particion->fin, particion->tamanio, particion->estaOcupado, pcb->pid, pcb->tid);
        }
    }
}

t_pcb_memoria *buscar_pcb_tid_por_particion(t_particion *particion)
{
    for (int i = 0; i < list_size(tablaPCBs); i++)
    {
        t_pcb_memoria *pcb = list_get(tablaPCBs, i);
        if (pcb->base == particion->inicio && pcb->limite == particion->fin)
        {
            return pcb;
        }
    }
    return NULL;
}

void liberar_hilo(t_pcb_memoria *pcb)
{
    list_remove_element(tablaPCBs, pcb);
    list_destroy_and_destroy_elements(pcb->instrucciones, free);
    free(pcb);
}