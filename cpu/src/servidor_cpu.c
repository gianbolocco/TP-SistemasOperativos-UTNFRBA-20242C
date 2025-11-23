#include "servidor_cpu.h"

void iniciar_servidor_cpu()
{
    initialize_contexto();
    pthread_create(&thr_server_interrupt, NULL, comenzar_servidor_interrupt, NULL);
    pthread_create(&thr_server_dispatch, NULL, comenzar_servidor_dispatch, NULL);
    pthread_detach(thr_server_interrupt);
    pthread_detach(thr_server_dispatch);
}

void finalizar_servidor_cpu()
{
    if (socket_server_interrupt != -1)
    {
        socket_destroy(socket_server_interrupt);
    }
    if (socket_server_dispatch != -1)
    {
        socket_destroy(socket_server_dispatch);
    }
}

void *comenzar_servidor_dispatch()
{
    socket_server_dispatch = server_start(config_cpu->puerto_dispatch, logger_aux);
    if (socket_server_dispatch == -1)
    {
        log_error(logger_aux, "Error al iniciar servidor de dispatch");
        flag_fin_programa = true;
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        pthread_exit(0);
    }
    int socket_cliente = client_wait(socket_server_dispatch, logger_aux);
    if (socket_cliente == -1)
    {
        log_error(logger_aux, "Error al aceptar conexion con el modulo de dispatch");
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        pthread_exit(0);
    }
    int hs_res = hs_server_to_module_valid(socket_cliente, HSCPU, logger_aux);
    if (hs_res == -1)
    {
        log_error(logger_aux, "Error al realizar handshake con el modulo de dispatch");
        flag_fin_programa = true;
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        socket_destroy(socket_server_interrupt);
        pthread_exit(0);
    }
    while (!flag_fin_programa)
    {
        recibir_hilo_a_ejecutar(socket_cliente);
        flag_ejecutando = true;
        obtener_contexto();
        ejecutar_hilo();
        actualizar_contexto();
        enviar_motivo_de_desalojo(socket_cliente);
        clean_contexto();
        flag_ejecutando = false;
        flag_desalojo = false;
        flag_interrupt = false;
        motivo_desalojo = CONTINUE;
    }
    socket_destroy(socket_cliente);
    socket_destroy(socket_server_dispatch);
    pthread_exit(0);
}

void *comenzar_servidor_interrupt()
{
    socket_server_interrupt = server_start(config_cpu->puerto_interrupt, logger_aux);
    if (socket_server_interrupt == -1)
    {
        log_error(logger_aux, "Error al iniciar servidor de interrupt");
        flag_fin_programa = true;
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        pthread_exit(0);
    }
    int socket_cliente = client_wait(socket_server_interrupt, logger_aux);
    if (socket_cliente == -1)
    {
        log_error(logger_aux, "Error al aceptar conexion con el modulo de interrupt");
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        pthread_exit(0);
    }
    int hs_res = hs_server_to_module_valid(socket_cliente, HSCPUINTERRUPTION, logger_aux);
    if (hs_res == -1)
    {
        log_error(logger_aux, "Error al realizar handshake con el modulo de interrupt");
        flag_fin_programa = true;
        flag_desalojo = true;
        flag_interrupt = true;
        motivo_desalojo = SEGFAULT;
        socket_destroy(socket_server_interrupt);
        pthread_exit(0);
    }
    while (!flag_fin_programa)
    {
        t_package *paquete = recibir_paquete(socket_cliente);
        if (paquete->codigo_operacion == FIN_DE_CLOCK)
        {
            if (flag_ejecutando)
            {
                log_info(logger, "## Llega interrupcion al puerto interrupt");
                flag_interrupt = true;
            }
        }
        eliminar_paquete(paquete);
    }
    socket_destroy(socket_cliente);
    socket_destroy(socket_server_interrupt);
    pthread_exit(0);
}
