#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_INI 5381
#define HASH_FACTOR 33


typedef enum {
    ERROR = 0,
    OK = 1
} Status;

typedef enum {
    VARIABLE,
    PARAMETRO,
    FUNCION
} Kind;

typedef enum {
    BOOLEAN,
    INT
} Type;

typedef enum {
    ESCALAR,
    VECTOR
} Category;

typedef struct _INFO {
    char* key;
    Kind kind;
    Type type;
    Category category;
    int size;
    int aux1;   
    int aux2;
} INFO;

typedef struct _NODE {
    INFO *info;
    struct _NODE *next;
} NODE;

typedef struct _HASH_TABLE {
    int size;
    NODE **node;
} HASH_TABLE;

INFO* create_symbol_info(char* key, Kind kind, Type type, Category category, int size, int aux1, int aux2);
NODE *create_node(INFO* info);
HASH_TABLE *create_hash_table(int size);

INFO* search_symbol(HASH_TABLE* table, char* key);
Status insert_symbol(HASH_TABLE* table, char* key, Kind kind, Type type, Category category, int size, int aux1, int aux2);
void delete_symbol(HASH_TABLE* table, char* key);

void free_info(INFO* info);
void free_node(NODE* node);
void free_hash_table(HASH_TABLE* table);

#endif