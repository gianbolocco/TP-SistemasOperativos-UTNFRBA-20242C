#include "ciclo_instruccion.h"

void fetch()
{
    log_info(logger, "## TID: %d - FETCH - Program Counter: %d", contexto->tid, contexto->pc);
    obtener_intruccion();
    contexto->pc++;
}

void decode()
{	/*
    char *instruccion_aux = string_duplicate(contexto->instruccion);
    free(contexto->instruccion);
    char **instruccion_parametros = string_split(instruccion_aux, " ");
    free(instruccion_aux);
    char *instruccion = string_array_replace(instruccion_parametros, 0, NULL);
    contexto->instruccion = instruccion;
    char *instruccion_parametros_aux = string_substring(instruccion_aux, strlen(instruccion) + 1, strlen(instruccion_aux) - strlen(instruccion));
    
    string_array_destroy(instruccion_parametros);
    if (contexto->instruccion_parametros != NULL)
		string_array_destroy(contexto->instruccion_parametros);

    contexto->instruccion_parametros = string_split(instruccion_parametros_aux, " ");

    free(instruccion_parametros_aux);
    */
	char **instruccion_separado = string_n_split(contexto->instruccion, 4, " ");
	contexto->instruccion_parametros = string_array_new();

	free(contexto->instruccion);
	contexto->instruccion = string_duplicate(instruccion_separado[0]);

	int cant_params = string_array_size(instruccion_separado) - 1;
	for(int i = 1; i <= cant_params; i++) {
		string_array_push(&contexto->instruccion_parametros, string_duplicate(instruccion_separado[i]));
	}
	string_array_destroy(instruccion_separado);
    
    decode_instruccion_tipo();
    
    switch (contexto->instruccion_tipo)
    {
    case INSTR_READ_MEM:
        contexto->direccion_fisica = mmu(contexto->instruccion_parametros[1]);
        break;
    case INSTR_WRITE_MEM:
        contexto->direccion_fisica = mmu(contexto->instruccion_parametros[0]);
        break;
    default:
        break;
    }
}

void execute()
{
    switch (contexto->instruccion_tipo)
    {
    case SET:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0], contexto->instruccion_parametros[1]);
        _set();
        break;
    case INSTR_READ_MEM:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0], contexto->instruccion_parametros[1]);
        read_mem();
        break;
    case INSTR_WRITE_MEM:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0], contexto->instruccion_parametros[1]);
        write_mem();
        break;
    case SUM:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0], contexto->instruccion_parametros[1]);
        _sum();
        break;
    case SUB:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0], contexto->instruccion_parametros[1]);
        _sub();
        break;
    case JNZ:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0], contexto->instruccion_parametros[1]);
        _jnz();
        break;
    case LOG:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0]);
        _log();
        break;
    case THREAD_EXIT:
        log_info(logger, "## TID: %d - Ejecutando: %s", contexto->tid, contexto->instruccion);
        flag_desalojo = true;
        check_desalojo(M_THREAD_EXIT);
        break;
    case PROCESS_EXIT:
        log_info(logger, "## TID: %d - Ejecutando: %s", contexto->tid, contexto->instruccion);
        flag_desalojo = true;
        check_desalojo(M_PROCESS_EXIT);
        break;
    case DUMP_MEMORY:
        log_info(logger, "## TID: %d - Ejecutando: %s", contexto->tid, contexto->instruccion);
        flag_desalojo = true;
        check_desalojo(M_DUMP_MEMORY);
        break;
    case INSTR_IO:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0]);
        flag_desalojo = true;
        check_desalojo(M_IO);
        break;
    case PROCESS_CREATE:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s %s %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0], contexto->instruccion_parametros[1], contexto->instruccion_parametros[2]);
        flag_desalojo = true;
        check_desalojo(M_PROCESS_CREATE);
        break;
    case THREAD_CREATE:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0], contexto->instruccion_parametros[1]);
        flag_desalojo = true;
        check_desalojo(M_THREAD_CREATE);
        break;
    case INSTR_THREAD_JOIN:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0]);
        flag_desalojo = true;
        check_desalojo(M_THREAD_JOIN);
        break;
    case THREAD_CANCEL:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0]);
        flag_desalojo = true;
        check_desalojo(M_THREAD_CANCEL);
        break;
    case MUTEX_CREATE:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0]);
        flag_desalojo = true;
        check_desalojo(M_MUTEX_CREATE);
        break;
    case MUTEX_LOCK:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0]);
        flag_desalojo = true;
        check_desalojo(M_MUTEX_LOCK);
        break;
    case MUTEX_UNLOCK:
        log_info(logger, "## TID: %d - Ejecutando: %s - %s", contexto->tid, contexto->instruccion, contexto->instruccion_parametros[0]);
        flag_desalojo = true;
        check_desalojo(M_MUTEX_UNLOCK);
        break;
    default:
        flag_desalojo = true;
        check_desalojo(SEGFAULT);
        break;
    }
}

void ejecutar_hilo()
{
    do
    {
        fetch();
        decode();
        execute();
        

        if (!flag_desalojo && !flag_interrupt && !flag_fin_programa)
        {
            free(contexto->instruccion);
            contexto->instruccion = NULL;
            string_array_destroy(contexto->instruccion_parametros);
            contexto->instruccion_parametros = NULL;
        }
    } while (!flag_desalojo && !flag_interrupt && !flag_fin_programa);
    if (flag_interrupt)
    {
        if (motivo_desalojo == CONTINUE)
        {
            motivo_desalojo = INTERRUPCION;
        }
        // flag_interrupt = false;
    }
}
