#include "utils.h"

void set_registry(char *registro, uint32_t value)
{
    log_debug(logger_aux, "Registro: %s - Valor antes de setear: %d - Valor a setear: %d", registro, get_registry(registro), value);
    if (strcmp(registro, "AX") == 0)
    {
        contexto->ax = value;
    }
    else if (strcmp(registro, "BX") == 0)
    {
        contexto->bx = value;
    }
    else if (strcmp(registro, "CX") == 0)
    {
        contexto->cx = value;
    }
    else if (strcmp(registro, "DX") == 0)
    {
        contexto->dx = value;
    }
    else if (strcmp(registro, "EX") == 0)
    {
        contexto->ex = value;
    }
    else if (strcmp(registro, "FX") == 0)
    {
        contexto->fx = value;
    }
    else if (strcmp(registro, "GX") == 0)
    {
        contexto->gx = value;
    }
    else if (strcmp(registro, "HX") == 0)
    {
        contexto->hx = value;
    }
    else
    {
        log_error(logger_aux, "Registro invalido: %s", registro);
        flag_desalojo = true;
        motivo_desalojo = SEGFAULT;
    }
}

uint32_t get_registry(char *registro)
{
    if (strcmp(registro, "AX") == 0)
    {
        return (contexto->ax);
    }
    else if (strcmp(registro, "BX") == 0)
    {
        return (contexto->bx);
    }
    else if (strcmp(registro, "CX") == 0)
    {
        return (contexto->cx);
    }
    else if (strcmp(registro, "DX") == 0)
    {
        return (contexto->dx);
    }
    else if (strcmp(registro, "EX") == 0)
    {
        return (contexto->ex);
    }
    else if (strcmp(registro, "FX") == 0)
    {
        return (contexto->fx);
    }
    else if (strcmp(registro, "GX") == 0)
    {
        return (contexto->gx);
    }
    else if (strcmp(registro, "HX") == 0)
    {
        return (contexto->hx);
    }
    else
    {
        log_error(logger_aux, "Registro invalido: %s", registro);
        flag_desalojo = true;
        motivo_desalojo = SEGFAULT;
        return 0;
    }
}

void decode_instruccion_tipo()
{
    if (strcmp(contexto->instruccion, "SET") == 0)
    {
        contexto->instruccion_tipo = SET;
    }
    else if (strcmp(contexto->instruccion, "READ_MEM") == 0)
    {
        contexto->instruccion_tipo = INSTR_READ_MEM;
    }
    else if (strcmp(contexto->instruccion, "WRITE_MEM") == 0)
    {
        contexto->instruccion_tipo = INSTR_WRITE_MEM;
    }
    else if (strcmp(contexto->instruccion, "SUM") == 0)
    {
        contexto->instruccion_tipo = SUM;
    }
    else if (strcmp(contexto->instruccion, "SUB") == 0)
    {
        contexto->instruccion_tipo = SUB;
    }
    else if (strcmp(contexto->instruccion, "JNZ") == 0)
    {
        contexto->instruccion_tipo = JNZ;
    }
    else if (strcmp(contexto->instruccion, "LOG") == 0)
    {
        contexto->instruccion_tipo = LOG;
    }
    else if (strcmp(contexto->instruccion, "DUMP_MEMORY") == 0)
    {
        contexto->instruccion_tipo = DUMP_MEMORY;
    }
    else if (strcmp(contexto->instruccion, "IO") == 0)
    {
        contexto->instruccion_tipo = INSTR_IO;
    }
    else if (strcmp(contexto->instruccion, "PROCESS_CREATE") == 0)
    {
        contexto->instruccion_tipo = PROCESS_CREATE;
    }
    else if (strcmp(contexto->instruccion, "PROCESS_EXIT") == 0)
    {
        contexto->instruccion_tipo = PROCESS_EXIT;
    }
    else if (strcmp(contexto->instruccion, "THREAD_CREATE") == 0)
    {
        contexto->instruccion_tipo = THREAD_CREATE;
    }
    else if (strcmp(contexto->instruccion, "THREAD_EXIT") == 0)
    {
        contexto->instruccion_tipo = THREAD_EXIT;
    }
    else if (strcmp(contexto->instruccion, "THREAD_JOIN") == 0)
    {
        contexto->instruccion_tipo = INSTR_THREAD_JOIN;
    }
    else if (strcmp(contexto->instruccion, "THREAD_CANCEL") == 0)
    {
        contexto->instruccion_tipo = THREAD_CANCEL;
    }
    else if (strcmp(contexto->instruccion, "MUTEX_CREATE") == 0)
    {
        contexto->instruccion_tipo = MUTEX_CREATE;
    }
    else if (strcmp(contexto->instruccion, "MUTEX_LOCK") == 0)
    {
        contexto->instruccion_tipo = MUTEX_LOCK;
    }
    else if (strcmp(contexto->instruccion, "MUTEX_UNLOCK") == 0)
    {
        contexto->instruccion_tipo = MUTEX_UNLOCK;
    }
    else
    {
        contexto->instruccion_tipo = UNKNOWN_INSTRUCTION;
        flag_desalojo = true;
        motivo_desalojo = SEGFAULT;
    }
}

void check_desalojo(t_motivo_desalojo nuevo_motivo)
{
    if (flag_interrupt)
    {
        flag_desalojo = true;
    }
    motivo_desalojo = nuevo_motivo;
}

void initialize_contexto()
{
    contexto = malloc(sizeof(t_contexto_cpu));
    contexto->pid = 0;
    contexto->tid = 0;
    contexto->pc = 0;
    contexto->ax = 0;
    contexto->bx = 0;
    contexto->cx = 0;
    contexto->dx = 0;
    contexto->ex = 0;
    contexto->fx = 0;
    contexto->gx = 0;
    contexto->hx = 0;
    contexto->base = 0;
    contexto->limite = 0;
    contexto->direccion_fisica = 0;
    contexto->instruccion_tipo = UNKNOWN_INSTRUCTION;
    contexto->instruccion = NULL;
    contexto->instruccion_parametros = NULL;
}

void clean_contexto()
{
    contexto->pid = 0;
    contexto->tid = 0;
    contexto->pc = 0;
    contexto->ax = 0;
    contexto->bx = 0;
    contexto->cx = 0;
    contexto->dx = 0;
    contexto->ex = 0;
    contexto->fx = 0;
    contexto->gx = 0;
    contexto->hx = 0;
    contexto->base = 0;
    contexto->limite = 0;
    contexto->direccion_fisica = 0;
    contexto->instruccion_tipo = UNKNOWN_INSTRUCTION;
    if (contexto->instruccion != NULL)
    {
        free(contexto->instruccion);
        contexto->instruccion = NULL;
    }
    if (contexto->instruccion_parametros != NULL)
    {
        string_array_destroy(contexto->instruccion_parametros);
        contexto->instruccion_parametros = NULL;
    }
}

void destroy_contexto()
{
    if (contexto != NULL)
    {
        clean_contexto();
        free(contexto);
    }
}

char *motivo_desalojo_to_string(t_motivo_desalojo motivo)
{
    switch (motivo)
    {
    case SEGFAULT:
        return "Segmentation Fault";
    case INTERRUPCION:
        return "Interrupcion";
    case M_IO:
        return "IO";
    case M_PROCESS_CREATE:
        return "Process Create";
    case M_THREAD_CREATE:
        return "Thread Create";
    case M_THREAD_JOIN:
        return "Thread Join";
    case M_THREAD_CANCEL:
        return "Thread Cancel";
    case M_MUTEX_CREATE:
        return "Mutex Create";
    case M_MUTEX_LOCK:
        return "Mutex Lock";
    case M_MUTEX_UNLOCK:
        return "Mutex Unlock";
    case M_PROCESS_EXIT:
        return "Process Exit";
    case M_THREAD_EXIT:
        return "Thread Exit";
    case M_DUMP_MEMORY:
        return "Dump Memory";
    case CONTINUE:
        return "Continue";
    default:
        return "Unknown";
    }
}