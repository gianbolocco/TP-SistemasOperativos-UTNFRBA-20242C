#include "esquemas.h"

t_bitarray *bitmap;
char *mappeo_bitmap, *mappeo_bloques;
FILE *archivo_bitmap, *archivo_bloques;
int bitmap_fildes, bloques_fildes;
pthread_mutex_t mutex_bitmap, mutex_bloques, mutex_mainLogger, mutex_auxLogger;
;

void cambiar_bit(t_bitarray *bitmap, int indice, bool valor)
{
	if (valor)
	{
		pthread_mutex_lock(&mutex_bitmap);
		bitarray_set_bit(bitmap, indice);
		msync(mappeo_bitmap + indice, 1, MS_SYNC);
		pthread_mutex_unlock(&mutex_bitmap);
		return;
	}

	pthread_mutex_lock(&mutex_bitmap);
	bitarray_clean_bit(bitmap, indice);
	msync(mappeo_bitmap + indice, 1, MS_SYNC);
	pthread_mutex_unlock(&mutex_bitmap);
}
