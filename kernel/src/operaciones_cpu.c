#include "operaciones_cpu.h"

void iniciar_conexion_cpu_dispath()
{
    socket_cpu_dispatch = create_connection(config_kernel->IP_CPU, config_kernel->PUERTO_CPU_DISPATCH, logger_aux);
    hs_client_to_module_valid(socket_cpu_dispatch, HSKERNEL, HSCPU, logger_aux);
}

void iniciar_conexion_cpu_interrupt()
{
    socket_cpu_interrupt = create_connection(config_kernel->IP_CPU, config_kernel->PUERTO_CPU_INTERRUPT, logger_aux);
    hs_client_to_module_valid(socket_cpu_interrupt, HSKERNEL, HSCPUINTERRUPTION, logger_aux);
}

void finalizar_conexion_cpu()
{
    if (socket_cpu_interrupt != -1)
        socket_destroy(socket_cpu_interrupt);
    if (socket_cpu_dispatch != -1)
        socket_destroy(socket_cpu_dispatch);
}

void enviar_hilo_a_cpu()
{
    t_package *paquete = crear_paquete(EJECUCION_HILO, buffer_vacio());
    agregar_a_paquete(paquete, &(hilo_en_exec->pid), sizeof(uint32_t));
    agregar_a_paquete(paquete, &(hilo_en_exec->tid), sizeof(uint32_t));
    enviar_paquete(paquete, socket_cpu_dispatch);
    eliminar_paquete(paquete);
}

void enviar_desalojo_a_cpu()
{
    t_package *paquete = crear_paquete(FIN_DE_CLOCK, buffer_vacio());
    enviar_paquete(paquete, socket_cpu_interrupt);
    eliminar_paquete(paquete);
}