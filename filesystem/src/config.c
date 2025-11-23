#include "config.h"

t_fs_config *config;

t_fs_config *read_config(char *path)
{
	t_config *cfg = config_create(path);
	if (!cfg)
		return NULL;
	t_fs_config *fs_config = malloc(sizeof(t_fs_config));
	fs_config->puerto_escucha = config_get_int_value(cfg, "PUERTO_ESCUCHA");
	fs_config->mount_dir = string_duplicate(config_get_string_value(cfg, "MOUNT_DIR"));
	fs_config->block_size = config_get_int_value(cfg, "BLOCK_SIZE");
	fs_config->block_count = config_get_int_value(cfg, "BLOCK_COUNT");
	fs_config->retardo_acceso_bloque = config_get_int_value(cfg, "RETARDO_ACCESO_BLOQUE");
	fs_config->log_level = string_duplicate(config_get_string_value(cfg, "LOG_LEVEL"));

	config_destroy(cfg);
	return fs_config;
}

void destroy_config(t_fs_config *config)
{
	free(config->log_level);
	free(config->mount_dir);
	free(config);
}
