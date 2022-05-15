#include "hash_table.h"

/*  Dada la información de un simbolo se crea la variable
    info, contenedora de toda la información necesaria para
    manejar un simbolo*/
INFO* create_symbol_info(char* key, Kind kind, Type type, Category category, int size, int aux1, int aux2) {
    INFO* info;

    info = (INFO*)malloc (sizeof(INFO));
    if(!info) {
        return NULL;
    }

    info->key = strdup(key);
    if(!info->key) {
        free(info);
        return NULL;
    }

    info->kind = kind;
    info->type = type;
    info->category = category;
    info->size = size;
    info->aux1 = aux1;
    info->aux2 = aux2;

    return info;
}

/*  Para mantener los elementos ordenados y localizables hemos
    implementado un mecanismo de listas formadas por nodos que nos
    permiten navegar de un simbolo al siguiente de forma ordenada*/
NODE *create_node(INFO* info) {
    NODE *node;

    if (!info) {
        printf("Sin info.\n");
        return NULL;
    }

    node = (NODE*)malloc(sizeof(NODE));
    if(!node) {
        return NULL;
    }

    node->info = info;
    node->next = NULL;

    return node;
}

/*  Funcion encargada de la inicialización de una tabla
    hash de tamanyo "size"*/
HASH_TABLE *create_hash_table(int size) {
    HASH_TABLE* table;

    if(size < 1) {
        printf("Tamaño menor que 1.");
        return NULL;
    }

    table = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));
    if(!table) {
        return NULL;
    }

    table->node = (NODE**)calloc(size, sizeof(NODE*));
    if(!table->node) {
        free(table);
        return NULL;
    }

    table->size = size; 

    return table;
}

/*  Devuelve el hash de la key*/
int hash(HASH_TABLE* table, char* key) {
    unsigned long val = HASH_INI;
    unsigned char* aux;

    for(aux = (unsigned char *)key; *aux; aux++) {
        val = val*HASH_FACTOR + *aux;
    }

    return val % table->size;
}

/*  Busca un simbolo mediandte la key */
INFO* search_symbol(HASH_TABLE* table, char* key) {
    int index;
    NODE* node = NULL;
    if (!table || !key) return NULL;
    index = hash(table, key);
    node = table->node[index];

    while(node != NULL) {
        if(node->info == NULL) break;
        if (strcmp(node->info->key, key) == 0) return node->info;
        node = node->next;  
    }

    if(!node || !node->info || strcmp(node->info->key, key) != 0) {
        return NULL;
    } else {
        return node->info;
    }

}

/*  Inserta un simbolo en la tabla creaandolo a partir de la informacion pasada como argumento*/
Status insert_symbol(HASH_TABLE* table, char* key, Kind kind, Type type, Category category, int size, int aux1, int aux2) {
    int index;
    INFO* info;
    NODE* node = NULL;

    if(!table) return ERROR;

    if(search_symbol(table, key) != NULL) {
        return ERROR;
    }

    index = hash(table, key);
    
    info = create_symbol_info(key, kind, type, category, size, aux1, aux1);

    if (!info) {
        return ERROR;
    }

    node = create_node(info);
    if (!node) {
        free_info(info);
        return ERROR;
    }

    node->next = table->node[index];
    table->node[index] = node;

    return OK;
}

/*  Elimina la key de la tabla y reordena la tabla hash*/
void delete_symbol(HASH_TABLE* table, char* key) {
    int val;
    NODE* next;
    NODE* last;
    NODE* aux;

    if(!table || !key) return;

    val = hash(table, key);
    next = table->node[val];

    while(next != NULL && next->info != NULL && strcmp(next->info->key, key) != 0) {
        last = next;
        next = next->next;
    }

    if(next != NULL && next->info != NULL && strcmp(next->info->key, key) == 0) {
        aux = next;
        free_info(next->info);
        free_node(next);

        if (aux == table->node[val]) {
            table->node[val] = aux->next;
        }
        else if (aux->next == NULL) {
            last->next = NULL;
        }
        else {
            last->next = aux->next;
        }

    }

}

/*  Sección encargada de liberar la memoria alojada para
    los componentes utilizados  */

void free_info(INFO* info) {
    if(info) {
        if(info->key) {
            free(info->key);
        }

        free(info);
    }
}

void free_node(NODE* node) {
    if(node != NULL) {
        if(node->info) free_info(node->info);
        free(node);
    }
}

void free_hash_table(HASH_TABLE* table) {
    int i;
    if (table != NULL){
        for(i = 0; i < table->size; i++) {
            free_node(table->node[i]);
        }
        free(table->node);
        free(table);
    }
}