#include "inicializar_estructuras.h"

void abrir_bitmap(void) {
	char *path = string_duplicate(config->mount_dir);

	string_append(&path, "/bitmap.dat");
	char archivo_no_existe = access(path, F_OK);
	archivo_bitmap = fopen(path, "a+");
	if(!archivo_bitmap) {
		log_error(logger_aux, "Error al abrir archivo bitmap.dat");
		exit(EXIT_FAILURE);
	}

	bitmap_fildes = fileno(archivo_bitmap);

	int cant_bytes_bitmap = (int)ceil(config->block_count/8.0);

	ftruncate(bitmap_fildes, cant_bytes_bitmap);
	mappeo_bitmap = mmap(0, cant_bytes_bitmap, PROT_WRITE | PROT_READ, MAP_SHARED, bitmap_fildes, 0);

	bitmap = bitarray_create_with_mode(mappeo_bitmap, cant_bytes_bitmap, LSB_FIRST);

	int cant_bits_bitmap = bitarray_get_max_bit(bitmap);

	if(archivo_no_existe) {
		reset_bitmap(cant_bits_bitmap);

		//pthread_mutex_lock(&mutex_mainLogger);	
		//log_info(logger, "## Archivo Creado: <bitmap.dat> - Tamaño: <%d>", cant_bytes_bitmap);
    	//pthread_mutex_unlock(&mutex_mainLogger);
	}
	/*
	bitarray_set_bit(bitmap, 7);
	bitarray_set_bit(bitmap, 16);
	*/
	free(path);

}

void abrir_bloques(void) {
	char *path = string_duplicate(config->mount_dir);
	string_append(&path, "/bloques.dat");
	char archivo_no_existe = access(path, F_OK);

	archivo_bloques = fopen(path, "a+");
	if(!archivo_bloques) {
		log_error(logger_aux, "Error al abrir archivo bloques.dat");
		exit(EXIT_FAILURE);
	}

	bloques_fildes = fileno(archivo_bloques);

	int size_espacio_bloques = config->block_count * config->block_size; // en bytes

	ftruncate(bloques_fildes, size_espacio_bloques);
	mappeo_bloques = mmap(0,size_espacio_bloques, PROT_WRITE | PROT_READ, MAP_SHARED, bloques_fildes, 0);

	if(archivo_no_existe) {
		memset(mappeo_bloques, 0, size_espacio_bloques);
		msync(mappeo_bloques, size_espacio_bloques, MS_SYNC);
		//pthread_mutex_lock(&mutex_mainLogger);	
		//log_info(logger, "## Archivo Creado: <bloques.dat> - Tamaño: <%d>", size_espacio_bloques);
    	//pthread_mutex_unlock(&mutex_mainLogger);
	}
	/*
	char *str = "probando ...";
	memcpy(mappeo_bloques + 120, str, strlen(str) +1);
	msync(mappeo_bloques + 120, strlen(str) +1, MS_SYNC);

	int n = 42;
	memcpy(mappeo_bloques + 221, &n, sizeof(int));
	msync(mappeo_bloques + 221, sizeof(int), MS_SYNC);
	*/
	free(path);
}

void iniciar_fs(void) {
	pthread_mutex_init(&mutex_bitmap, NULL);
	pthread_mutex_init(&mutex_bloques, NULL);
	pthread_mutex_init(&mutex_mainLogger, NULL);
	pthread_mutex_init(&mutex_auxLogger, NULL);
	abrir_bitmap();
	abrir_bloques();
}

void reset_bitmap(int bits_size) {
	int offset = 0, count = 0;
	for(int i = 0; i < bits_size; i++, count++) {
		if(count == 8) {
			offset += 1;
			count = 0;
		}
		cambiar_bit(bitmap, i, 0);
	}
}
