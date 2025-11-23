#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include <commons/config.h>
#include <stdlib.h>
#include <commons/string.h>

typedef struct {
	int puerto_escucha;
	char* mount_dir;
	int block_size;
	int block_count;
	int retardo_acceso_bloque;
	char* log_level;
} t_fs_config;

extern t_fs_config* config;

t_fs_config *read_config(char*);
void destroy_config(t_fs_config*);

#endif /* SRC_CONFIG_H_ */
