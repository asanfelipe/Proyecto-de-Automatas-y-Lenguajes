#include "symbols_table.h"

#define TOTAL_SIZE_GLOBAL 50
#define TOTAL_SIZE_LOCAL 50

HASH_TABLE *global_table = NULL;
HASH_TABLE *local_table = NULL;

/*  Funcion clasificadora de variables declaradas
    En caso de estar en un ambito local se utilizara
    dicha manera de declaracion, en caso de no haber
    ambito local se utilizara el global*/
Status declare(char *id,  INFO* desc_id) {
    if (local_table == NULL) {
       return declare_global(id, desc_id);
    } else {
       return declare_local(id, desc_id);
    }
}

/*  Declaracion de simbolo en el ambito global*/
Status declare_global(char *id, INFO* desc_id){

    if(!id) return ERROR;

    if (global_table == NULL) {
        global_table = create_hash_table(TOTAL_SIZE_GLOBAL);
        if(!global_table) {
            return ERROR;
        }
    }

    if(search_symbol(global_table, id) == NULL) {
        if(insert_symbol(global_table, id, desc_id->kind, desc_id->type, desc_id->category, desc_id->size, desc_id->aux1, desc_id->aux2) == ERROR) {
            return ERROR;
        }
    }else{
        return ERROR;
    }
    
    return OK;
}

/*  Declaracion de simbolo en el ambito local*/
Status declare_local(char *id, INFO* desc_id){
    if(!id || !desc_id) return ERROR;

    if (local_table == NULL) {
        local_table = create_hash_table(TOTAL_SIZE_LOCAL);
        if(!local_table) {
            return ERROR;
        }
    }

    if(search_symbol(local_table, id) == NULL) {
        if(insert_symbol(local_table, id, desc_id->kind, desc_id->type, desc_id->category, desc_id->size, desc_id->aux1, desc_id->aux2) == ERROR) {
            return ERROR;
        }
    }else{
        return ERROR;
    }
    
    return OK;
}

/*  Busqueda del id en el ambito global*/
INFO* global_search(char *id){

    if(!global_table || !id) {
        return NULL;
    }

    return search_symbol(global_table, id);
    
}

/*  Busqueda del id en el ambito local
    primero, en caso de encontrarlo se devuelve la info
    del id buscado en caso de no hacerlo se hace una busqueda
    global, si no se encuentra el simbolo se devuelve NULL*/
INFO* local_search(char *id){
    if(!id) return ERROR;

    if(local_table != NULL){
        if(search_symbol(local_table, id) == NULL) {
            return global_search(id);
        } else {
            return search_symbol(local_table, id);
        }
    }else{
        return global_search(id);
    }
    
    return NULL;
}

/*  Declaracion de una funcion y apertura con ella de un
    ambito local*/
Status declare_function(char *id, INFO *desc_id){

    if(!id || !desc_id) return ERROR;

    if(search_symbol(global_table, id) != NULL) {
       return ERROR;
    } else {
        insert_symbol(global_table, id, desc_id->kind, desc_id->type, desc_id->category, desc_id->size, desc_id->aux1, desc_id->aux2);  /*Set global table*/
        /* Table_local ini */
        if (!local_table) local_table = create_hash_table(TOTAL_SIZE_LOCAL);
        if (!local_table) {
            delete_symbol(global_table, id);
            return ERROR;
        }

        if(insert_symbol(local_table, id, desc_id->kind, desc_id->type, desc_id->category, desc_id->size, desc_id->aux1, desc_id->aux2) == ERROR) {
            delete_symbol(global_table, id);
            return ERROR;
        }
        return OK;
    }
}

/*  Cierre de una funcion o ambito local*/
void close() {

    if(!local_table) return;
    free_hash_table(local_table);
    local_table = NULL;
}

/*  Liberacion de los recursos*/
void free_tables() {
    free_hash_table(local_table);
    free_hash_table(global_table);
}