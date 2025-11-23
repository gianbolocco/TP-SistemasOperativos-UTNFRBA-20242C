#include "operaciones_memoria.h"

int iniciar_conexion_memoria()
{
    int socket_cliente = create_connection(config_kernel->IP_MEMORIA, config_kernel->PUERTO_MEMORIA, logger_aux);
    hs_client_to_module_valid(socket_cliente, HSKERNEL, HSMEMORIA, logger_aux);
    return socket_cliente;
}

void finalizar_conexion_memoria(int socket_cliente)
{
    socket_destroy(socket_cliente);
}

op_code crear_proceso_en_memoria(t_tcb *tcb_0, int socket_cliente)
{
    t_pcb *proceso = buscar_proceso(tcb_0->pid);
    t_package *paquete_envio = crear_paquete(CREAR_PROCESO, buffer_vacio());
    agregar_a_paquete(paquete_envio, &(tcb_0->pid), sizeof(uint32_t));
    agregar_a_paquete(paquete_envio, &(proceso->tamanio), sizeof(uint32_t));
    agregar_a_paquete(paquete_envio, tcb_0->archivo, string_length(tcb_0->archivo) + 1);
    enviar_paquete(paquete_envio, socket_cliente);
    eliminar_paquete(paquete_envio);
    t_package *paquete_respuesta = recibir_paquete(socket_cliente);
    op_code codigo_operacion = paquete_respuesta->codigo_operacion;
    eliminar_paquete(paquete_respuesta);
    return codigo_operacion;
}

op_code finalizar_proceso_en_memoria(t_pcb *proceso, int socket_cliente)
{
    t_package *paquete_envio = crear_paquete(FIN_PROCESO, buffer_vacio());
    agregar_a_paquete(paquete_envio, &(proceso->pid), sizeof(uint32_t));
    enviar_paquete(paquete_envio, socket_cliente);
    eliminar_paquete(paquete_envio);
    t_package *paquete_respuesta = recibir_paquete(socket_cliente);
    op_code codigo_operacion = paquete_respuesta->codigo_operacion;
    eliminar_paquete(paquete_respuesta);
    return codigo_operacion;
}

op_code crear_hilo_en_memoria(t_tcb *hilo, int socket_cliente)
{
    t_package *paquete_envio = crear_paquete(CREAR_HILO, buffer_vacio());
    agregar_a_paquete(paquete_envio, &(hilo->pid), sizeof(uint32_t));
    agregar_a_paquete(paquete_envio, &(hilo->tid), sizeof(uint32_t));
    agregar_a_paquete(paquete_envio, hilo->archivo, string_length(hilo->archivo) + 1);
    enviar_paquete(paquete_envio, socket_cliente);
    eliminar_paquete(paquete_envio);
    t_package *paquete_respuesta = recibir_paquete(socket_cliente);
    op_code codigo_operacion = paquete_respuesta->codigo_operacion;
    eliminar_paquete(paquete_respuesta);
    return codigo_operacion;
}

op_code finalizar_hilo_en_memoria(t_tcb *hilo, int socket_cliente)
{
    t_package *paquete_envio = crear_paquete(FIN_HILO, buffer_vacio());
    agregar_a_paquete(paquete_envio, &(hilo->pid), sizeof(uint32_t));
    agregar_a_paquete(paquete_envio, &(hilo->tid), sizeof(uint32_t));
    enviar_paquete(paquete_envio, socket_cliente);
    eliminar_paquete(paquete_envio);
    t_package *paquete_respuesta = recibir_paquete(socket_cliente);
    op_code codigo_operacion = paquete_respuesta->codigo_operacion;
    eliminar_paquete(paquete_respuesta);
    return codigo_operacion;
}

op_code ejecutar_dump_memory(t_tcb *hilo, int socket_cliente)
{
    t_package *paquete_envio = crear_paquete(MEMORY_DUMP, buffer_vacio());
    agregar_a_paquete(paquete_envio, &(hilo->pid), sizeof(uint32_t));
    agregar_a_paquete(paquete_envio, &(hilo->tid), sizeof(uint32_t));
    enviar_paquete(paquete_envio, socket_cliente);
    eliminar_paquete(paquete_envio);
    t_package *paquete_respuesta = recibir_paquete(socket_cliente);
    op_code codigo_operacion = paquete_respuesta->codigo_operacion;
    eliminar_paquete(paquete_respuesta);
    return codigo_operacion;
}