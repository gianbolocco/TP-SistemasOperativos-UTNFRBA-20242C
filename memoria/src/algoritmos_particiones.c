#include "algoritmos_particiones.h"

void inicializar_particiones_fijas()
{
    // Procesar los valores (convertirlos de strings a enteros)
    int i = 0;
    int inicio = 0;

    while (config_datos->particiones[i] != NULL)
    {
        int tamanioParticion = atoi(config_datos->particiones[i]); // el atoi me convierte el string en un entero y el itoa al revez
        t_particion *particion = malloc(sizeof(t_particion));
        particion->tamanio = tamanioParticion;
        particion->inicio = inicio;
        particion->fin = inicio + tamanioParticion - 1; // sumo mi tamaño + mi inicio y le resto 1 por el array position
        particion->estaOcupado = false;
        inicio = inicio + tamanioParticion;
        list_add(tablaParticiones, particion);
        // mi lista        //el elemento a agregar tpo el item
        i++;
    }
}

void inicializar_particiones_dinamicas()
{
    // yo aca le doy toda la memoria como primera particion y despues la voy subdividiendo para los proceos que me lleguen
    t_particion *particion = malloc(sizeof(t_particion));
    particion->tamanio = config_datos->tamanio_memoria;
    particion->inicio = 0;
    particion->fin = config_datos->tamanio_memoria - 1;
    particion->estaOcupado = false;
    list_add(tablaParticiones, particion);
}