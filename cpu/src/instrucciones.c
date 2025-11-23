#include "instrucciones.h"

void _set()
{
    char *registro = contexto->instruccion_parametros[0];
    uint32_t valor = (uint32_t)strtoul(contexto->instruccion_parametros[1], NULL, 10);
    set_registry(registro, valor);
    check_desalojo(CONTINUE);
}

void _sum()
{
    char *registro_destino = contexto->instruccion_parametros[0];
    char *registro_origen = contexto->instruccion_parametros[1];
    uint32_t destino = get_registry(registro_destino);
    uint32_t origen = get_registry(registro_origen);
    uint32_t res = destino + origen;
    set_registry(registro_destino, res);
    check_desalojo(CONTINUE);
}

void _sub()
{
    char *registro_destino = contexto->instruccion_parametros[0];
    char *registro_origen = contexto->instruccion_parametros[1];
    uint32_t destino = get_registry(registro_destino);
    uint32_t origen = get_registry(registro_origen);
    uint32_t res = destino - origen;
    set_registry(registro_destino, res);
    check_desalojo(CONTINUE);
}

void _jnz()
{
    char *registro = contexto->instruccion_parametros[0];
    int instruccion_pc = atoi(contexto->instruccion_parametros[1]);
    uint32_t valor = get_registry(registro);
    if (valor != 0)
    {
        contexto->pc = instruccion_pc;
    }
    check_desalojo(CONTINUE);
}

void _log()
{
    char *registro = contexto->instruccion_parametros[0];
    log_info(logger, "## Registro %s: %d", registro, get_registry(registro));
    check_desalojo(CONTINUE);
}

uint32_t mmu(char *registro)
{
    uint32_t desplazamiento = get_registry(registro);
    if (desplazamiento + sizeof(uint32_t) > contexto->limite)
    {
        log_error(logger_aux, "Segmentation Fault - Direccion Fisica: %d", desplazamiento);
        flag_desalojo = true;
        motivo_desalojo = SEGFAULT;
        return -1;
    }
    return contexto->base + desplazamiento;
}
