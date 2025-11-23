#include "archivo_dump.h"

void crear_archivo_dump(char *nombre, uint32_t size, void *contenido, int socket)
{

    int cant_bloques_requerido = (int)ceil((double)size / config->block_size);

    if (!verificar_espacio_bitmap(cant_bloques_requerido, nombre))
    { // si no hay espacio en bitmap termino
        enviar_respuesta_peticion(0, socket);

        pthread_mutex_lock(&mutex_mainLogger);
        log_info(logger, "## Fin de solicitud - Archivo: %s", nombre);
        pthread_mutex_unlock(&mutex_mainLogger);

        return;
    }

    t_list *indices_bloques = reservar_bitmap(cant_bloques_requerido, nombre);

    crear_archivo_metadata(nombre, size, *((uint32_t *)list_get(indices_bloques, 0)));

    actualizar_bloque_indices(indices_bloques, nombre);

    actualizar_bloque_datos(indices_bloques, contenido, size, nombre);

    enviar_respuesta_peticion(1, socket);
    pthread_mutex_lock(&mutex_mainLogger);
    log_info(logger, "## Fin de solicitud - Archivo: %s", nombre);
    pthread_mutex_unlock(&mutex_mainLogger);

    // en caso de segfaults comentar las funciones que liberan memoria aca abajo
    //free(nombre);
    //free(contenido);
    list_destroy_and_destroy_elements(indices_bloques, free);
}

bool verificar_espacio_bitmap(uint32_t cant_bloques_requerido, char *nombre)
{

    int cant_bloques = config->block_count;
    int cantidadLibre = 0;
    int cant_indices = config->block_size / sizeof(uint32_t);

    for (int offset = 0; offset < cant_bloques; offset++)
    {

        pthread_mutex_lock(&mutex_bitmap);
        bool bit = bitarray_test_bit(bitmap, offset);
        pthread_mutex_unlock(&mutex_bitmap);

        if (!bit)
        {
            cantidadLibre++;
        }
    }
    if (cant_indices < cant_bloques_requerido)
    {
        log_error(logger_aux, "[%s]:Archivo excede tamanio que acepta el FS. Limite cant bloques: %d Cant bloques solicitados: %d", nombre, cant_indices, cant_bloques_requerido);
        return false;
    }

    if (cantidadLibre < cant_bloques_requerido)
    {
        log_error(logger_aux, "[%s]:No hay suficiente espacio para %d bloques. Avisando a memoria", nombre, cant_bloques_requerido+1);
        return false;
    }

    pthread_mutex_lock(&mutex_auxLogger);
    log_info(logger_aux, "[%s]:Hay espacio para %d bloques del archivo.", nombre, cant_bloques_requerido);
    pthread_mutex_unlock(&mutex_auxLogger);

    return true;
}

t_list *reservar_bitmap(int cant_bloques_requerido, char *nombre)
{

    int cont = cant_bloques_requerido + 1;
    int cant_bloques = config->block_count;
    int cant_libres = 0;
    t_list *indice_bloques = list_create();

    for (int i = 0; i < cant_bloques; i++)
    {

        pthread_mutex_lock(&mutex_bitmap);
        bool bit = bitarray_test_bit(bitmap, i);
        pthread_mutex_unlock(&mutex_bitmap);

        if (!bit)
            cant_libres++;
    }

    uint32_t *indice;
    for (int i = 0; cont > 0; i++)
    {

        pthread_mutex_lock(&mutex_bitmap);
        bool bit = bitarray_test_bit(bitmap, i);
        pthread_mutex_unlock(&mutex_bitmap);

        if (!bit)
        {
            cambiar_bit(bitmap, i, 1); // reservo el bloque, marcando la posicion del bloque en bitmap

            cont--;

            indice = malloc(sizeof(uint32_t));
            *indice = i;

            list_add(indice_bloques, indice);

            pthread_mutex_lock(&mutex_mainLogger);
            log_info(logger, "## Bloque asignado: %d - Archivo: %s - Bloques Libres: %d", i, nombre, --cant_libres);
            pthread_mutex_unlock(&mutex_mainLogger);
        }
    }

    return indice_bloques;
}

void crear_archivo_metadata(char *nombre, uint32_t size, uint32_t indice_bloque_indice)
{ // indice_bloque_indice es el indice del bloque que contiene los indices de los bloques de datos

    char *path = string_duplicate(config->mount_dir);
    string_append(&path, "/files/");
    string_append(&path, nombre);
    t_config *config = malloc(sizeof(t_config));
    config->path = string_duplicate(path);
    config->properties = dictionary_create();
    char *size_str = string_itoa(size);
    char *indice_str = string_itoa(indice_bloque_indice);
    dictionary_put(config->properties, "SIZE", size_str);
    dictionary_put(config->properties, "INDEX_BLOCK", indice_str);
    config_save(config);
    log_info(logger, "## Archivo Creado: %s - Tamaño: %s", nombre, size_str);
    config_destroy(config);
    free(path);
}

void actualizar_bloque_indices(t_list *indices, char *nombre)
{

    uint32_t offset_inicio_indice = *((uint32_t *)list_get(indices, 0)) * config->block_size; // apunta al inicio del bloque de indice
    uint32_t offset_indice = 0;                                                               // apunta a la posicion de un valor de indice en particular del bloque de indice
    int cant_bloques_datos = list_size(indices) - 1;

    for (int i = 1; i <= cant_bloques_datos; i++, offset_indice += sizeof(uint32_t))
    {
        uint32_t contenido = *((uint32_t *)list_get(indices, i)) * config->block_size;

        pthread_mutex_lock(&mutex_bloques);
        memcpy(mappeo_bloques + offset_inicio_indice + offset_indice, &contenido, sizeof(uint32_t));
        msync(mappeo_bloques + offset_inicio_indice + offset_indice, sizeof(uint32_t), MS_SYNC);
        pthread_mutex_unlock(&mutex_bloques);
    }

    usleep(config->retardo_acceso_bloque * 1000);

    pthread_mutex_lock(&mutex_mainLogger);
    log_info(logger, "## Acceso Bloque - Archivo: %s - Tipo Bloque: ÍNDICE - Bloque File System %u", nombre, offset_inicio_indice);
    pthread_mutex_unlock(&mutex_mainLogger);
}

void actualizar_bloque_datos(t_list *indices, void *contenido, uint32_t size, char *nombre)
{
    int cant_bloques_datos = list_size(indices) - 1;
    int offset_dato, offset_contenido = 0;

    for (int i = 1; i <= cant_bloques_datos; i++)
    {
        uint32_t offset_inicio_bloque = *((uint32_t *)list_get(indices, i)) * config->block_size;
        uint32_t n;

        if (i == cant_bloques_datos)
            n = size - (i - 1) * config->block_size;
        else
            n = config->block_size;

        for (offset_dato = 0; offset_dato < n; offset_dato += sizeof(uint32_t))
        {
            pthread_mutex_lock(&mutex_bloques);
            memcpy(mappeo_bloques + offset_inicio_bloque + offset_dato, contenido + offset_contenido, sizeof(uint32_t));
            msync(mappeo_bloques + offset_inicio_bloque + offset_dato, sizeof(uint32_t), MS_SYNC);
            pthread_mutex_unlock(&mutex_bloques);

            log_debug(logger_aux, "Tiempo restante: %d", config->retardo_acceso_bloque);
            usleep(config->retardo_acceso_bloque * 1000);

            pthread_mutex_lock(&mutex_mainLogger);
            log_info(logger, "## Acceso Bloque - Archivo: %s - Tipo Bloque: DATOS - Bloque File System %u", nombre, *((uint32_t *)list_get(indices, i)));
            pthread_mutex_unlock(&mutex_mainLogger);

            offset_contenido += sizeof(uint32_t);
        }
    }
}

void enviar_respuesta_peticion(uint32_t resultado, int socket)
{ // resultado = 0 si hay error, 1 si se atendio la peticion sin problema

	t_package *paquete;
	if(resultado == 0)
		paquete = crear_paquete(MEMORY_DUMP, buffer_vacio());
	else
		paquete = crear_paquete(SUCCESS, buffer_vacio());

    enviar_paquete(paquete, socket);
    eliminar_paquete(paquete);
}
