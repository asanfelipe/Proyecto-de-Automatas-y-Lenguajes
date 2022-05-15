
#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H

#include "hash_table.h"

#define TABLA_SIMBOLOS_GLOBAL_TAM   65536
#define TABLA_SIMBOLOS_LOCAL_TAM    65536

STATUS Crear_Ambito(const char *id, INFO_SIMBOLO *simb);
STATUS Crear_Ambito_Global(const char *id, INFO_SIMBOLO *simb);
STATUS Crear_Ambito_Local(const char *id, INFO_SIMBOLO *simb);
INFO_SIMBOLO *Buscar_Global(const char *id);
INFO_SIMBOLO *Buscar_Local(const char *id);
STATUS Insertar_Simbolo(const char *id, INFO_SIMBOLO *simb);
STATUS Liberar_AmbitoLocal();
void Liberar();
int es_Local(const char *id);

#endif /* TABLASIMBOLOS_H */
