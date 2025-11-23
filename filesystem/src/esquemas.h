#ifndef SRC_ESQUEMAS_H_
#define SRC_ESQUEMAS_H_

#include <commons/bitarray.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "log.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

extern t_bitarray *bitmap;
extern char *mappeo_bitmap, *mappeo_bloques;
extern FILE *archivo_bitmap, *archivo_bloques;
extern int bitmap_fildes, bloques_fildes;
extern pthread_mutex_t mutex_bitmap, mutex_bloques, mutex_mainLogger, mutex_auxLogger;

void cambiar_bit(t_bitarray *, int, bool);

#endif /* SRC_ESQUEMAS_H_ */
