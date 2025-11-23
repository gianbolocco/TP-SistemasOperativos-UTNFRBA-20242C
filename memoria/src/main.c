#include "main.h"
// Este lo hago en todos los modulos
// primero prendo el fs, inicializo el cliente porque asi se que servicio HAGO LO MISMO PARA TODOS
// en kernel solo cliente
// en fs solo servidor
// mi libreria t list que ya tiene el indicie

int main(void)
{
    // Para cierre seguro
    signal(SIGINT, sigintHandler);

    // INICIALIZAR MEMORIA
    leer_configuracion();

    if (config_datos == NULL)
    {
        finalizar_programa();
        return EXIT_FAILURE;
    }

    // INICIALIZAR LOGS
    t_log_level nivel_log = log_level_from_string(config_datos->log_level);

    logger = log_create("memoria.log", "memoria", true, nivel_log);
    logger_aux = log_create("memoria_aux.log", "memoria", true, LOG_LEVEL_TRACE);

    // INICIALIZAR ESTRUCURA DE MEMORIA
    inicializar_estructuras_memoria();

    // INICIALIZAR SERVIDOR DE COMUNICACIONES
    iniciar_servidor_memoria();

    // Espero hasta que finalice el programa
    while (!flag_fin_programa)
    {
        sleep(1);
    }
    finalizar_servidor_memoria();
    if (memoria != NULL)
    {
        eliminar_estructuras_memoria();
    }
    finalizar_programa();

    return EXIT_SUCCESS;
}

// Permite liberar el socket de forma segura para evitar que el SO lo retenga por un minuto
void sigintHandler(int signum)
{
    log_warning(logger_aux, "Iniciando fin del modulo por signal de apagado");
    // end_memoria_server();
    // end_memoria_core();
    finalizar_programa();
    exit(signum);
}

void finalizar_programa()
{
    // asi no explota como mica
    eliminar_estructuras_memoria();
    destroy_configuracion();
    if (logger != NULL)
    {
        log_destroy(logger);
    }
    if (logger_aux != NULL)
    {
        log_destroy(logger_aux);
    }
}
