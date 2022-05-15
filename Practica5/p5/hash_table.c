#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

INFO_SIMBOLO *crear_info_simbolo(const char *lexema, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2) {
    INFO_SIMBOLO *info;

    if ((info = (INFO_SIMBOLO *) malloc(sizeof(INFO_SIMBOLO)))) {
        if (!(info->lexema = strdup(lexema))) {
            free(info);
            return NULL;
        }

        info->categoria = categ;
        info->tipo = tipo;
        info->clase = clase;
        info->adicional1 = adic1;
        info->adicional2 = adic2;
    }
    return info;
}


NODO_HASH *crear_nodo(INFO_SIMBOLO *is) {
    NODO_HASH *nodo;

    if ((nodo = (NODO_HASH *) malloc( sizeof( NODO_HASH )))) {
        nodo->siguiente = NULL;
        nodo->info = is;
    }

    return nodo;
}


TABLA_HASH *crear_tabla(int tam) {

    TABLA_HASH *hash;

    hash = (TABLA_HASH *) malloc(sizeof(TABLA_HASH));
    if (hash) {

        hash->tabla = (NODO_HASH **) calloc(tam, sizeof(NODO_HASH *));

        if (!(hash->tabla )) {
            free(hash);
            return NULL;
        }

        hash->tam = tam;
    }
    return hash;
}


unsigned long hash(const char *str) {
    unsigned long code;
    unsigned char *key;

    code = HASH_INI;

    for (key = (unsigned char *) str; *key; key++) {
        code *=  HASH_FACTOR + *key;
    }
    return code;
}


INFO_SIMBOLO *buscar_simbolo(const TABLA_HASH *th, const char *lexema) {
    unsigned long i;
    NODO_HASH *nodo;

    i = hash(lexema) % th->tam;

    nodo = th->tabla[i];
    while (nodo && ( !(nodo->info) || (strcmp(nodo->info->lexema, lexema) ))) {
        nodo = nodo->siguiente;
    }

    if(nodo){
      return nodo->info;
    }

    return NULL;
}


STATUS insertar_simbolo(TABLA_HASH *th, const char *lexema, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2) {
    unsigned long i;
    INFO_SIMBOLO *info;
    NODO_HASH *nodo = NULL;

    if (buscar_simbolo(th, lexema)) {
        return ERR;
    }
    i = hash(lexema) % th->tam;
    info = crear_info_simbolo(lexema, categ, tipo, clase, adic1, adic2);

    if (!info) {
        return ERR;
    }
    nodo = crear_nodo(info);

    if (!nodo) {
        liberar_info_simbolo(info);
        return ERR;
    }

    nodo->siguiente = th->tabla[i];
    th->tabla[i] = nodo;

    return OK;
}

void borrar_simbolo(TABLA_HASH *th, const char *lexema) {
    unsigned long i;
    NODO_HASH *nodo = NULL;
    NODO_HASH *aux = NULL;

    i = hash(lexema) % th->tam;
    nodo = th->tabla[i];

    while (nodo && ( !(nodo->info) || (strcmp(nodo->info->lexema, lexema)) )) {
        aux = nodo;
        nodo = nodo->siguiente;
    }
    if (!nodo){
      return;
    }
    if (!aux) {
        th->tabla[i] = nodo->siguiente;
    }
    else {
        aux->siguiente = nodo->siguiente;
    }
    liberar_nodo(nodo);
    return;
}

void liberar_info_simbolo(INFO_SIMBOLO *is) {
    if (is){
        if(is->lexema) {

            free(is->lexema);
        }
        free(is);
    }
}


void liberar_tabla(TABLA_HASH *th) {

    int i;
    NODO_HASH *aux1, *aux2;

    if (th){
        if (th->tabla) {
            for (i = 0; i < th->tam; i++) {

                aux1 = th->tabla[i];

                while (aux1) {

                    aux2 = aux1->siguiente;
                    liberar_nodo(aux1);
                    aux1 = aux2;
                }
            }
            free(th->tabla);
        }
        free(th);

    }
}

void liberar_nodo(NODO_HASH *nh) {

    if (nh) {
        liberar_info_simbolo(nh->info);
        free(nh);
    }
}