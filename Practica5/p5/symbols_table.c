#include "symbols_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TABLA_HASH *TablaGlobal = NULL;
TABLA_HASH *TablaLocal = NULL;

/** Declaramos una tabla global o local. Llamamos a global en caso de que esta
 * no exista. Sino llamamos a local. */
STATUS Crear_Ambito(const char *id, INFO_SIMBOLO *simb) {
  if (TablaLocal == NULL) {
    return Crear_Ambito_Global(id, simb);
  }
  return Crear_Ambito_Local(id, simb);
}

/* Creamos un ambito Global */
STATUS Crear_Ambito_Global(const char *id, INFO_SIMBOLO *simb) {

  /* Unicamente crearemos una tabla hash nueva en caso de que no exista con
   * anterioridad este ambito */
  if (!TablaGlobal) {
    TablaGlobal = crear_tabla(TABLA_SIMBOLOS_GLOBAL_TAM);

    if (!TablaGlobal) {
      return ERR;
    }
  }

  /* Buscamos el simbolo id en nuestra tabla de simbolos hash. Si este simbolo
   * ya se encuentra, pasamos de esta funcion. En caso de que no se encuentre lo
   * metemos en nuestra tabla */
  if (!buscar_simbolo(TablaGlobal, id)) {
    return insertar_simbolo(TablaGlobal, id, simb->categoria,
                            simb->tipo, simb->clase, simb->adicional1,
                            simb->adicional2);
  }

  return ERR;
}

/* Creamos un ambito Local */
STATUS Crear_Ambito_Local(const char *id, INFO_SIMBOLO *simb) {

  /* Buscamos el simbolo id en nuestra tabla Local de simbolos hash. Si este
   * simbolo ya se encuentra, pasamos de esta funcion. En caso de que no se
   * encuentre lo metemos en nuestra tabla */
  if (!buscar_simbolo(TablaLocal, id)) {
    return insertar_simbolo(TablaLocal, id, simb->categoria,
                            simb->tipo, simb->clase, simb->adicional1,
                            simb->adicional2);
  }

  return ERR;
}

/* Buscamos si existe dicho simbolo en un ambito global */
INFO_SIMBOLO *Buscar_Global(const char *id) {
  if (!TablaGlobal) {
    return NULL;
  }
  return buscar_simbolo(TablaGlobal, id);
}

/*  Buscamos si existe dicho simbolo en un ambito local o global */
INFO_SIMBOLO *Buscar_Local(const char *id) {
  INFO_SIMBOLO *dato;

  /* Al buscar un simbolo en nuestras tablas, primero tenemos que buscar si
   * pertenece al ambito local. Si no existe dicho ambito se buscara en el
   * ambito global. Tras buscar en el ambito Local, si no se encuentra dicho
   * simbolo ahí, se procederá a buscar en el ambito Global */
  if (!TablaLocal) {
    return Buscar_Global(id);
  }

  dato = buscar_simbolo(TablaLocal, id);
  if (!dato) {
    return Buscar_Global(id);
  }
  return dato;
}


/** Comprobamos si un simbolo se encuentra dentro de nuestro ambito global y procedemos a realizar las acciones necesarias si no lo hace */
STATUS Insertar_Simbolo(const char *id, INFO_SIMBOLO *simb) {

  /* Si no encontramos un simbolo, procedemos a insertarlo en nuestras tablas, borrando y creando las locales y más cosas */
  if (!buscar_simbolo(TablaGlobal, id)) {

    if (insertar_simbolo(TablaGlobal, id, simb->categoria,
                         simb->tipo, simb->clase, simb->adicional1,
                         simb->adicional2) == ERR) {
      return ERR;
    }

    /* Borramos la tabla Local y la volvemos a crear acto seguido*/
    liberar_tabla(TablaLocal);
    TablaLocal = crear_tabla(TABLA_SIMBOLOS_LOCAL_TAM);

    /* En caso de no poder crear la tabla de simbolos local, borramos todos los simbolos y borramos la tabla */
    if (!TablaLocal) {
      borrar_simbolo(TablaGlobal, id);
      liberar_tabla(TablaLocal);
      TablaLocal = NULL;
      return ERR;
    }

    /* Insertamos el simbolo en la tabla local. Si esta inserción causa error, libera elementos */
    if (insertar_simbolo(TablaLocal, id, simb->categoria,
                         simb->tipo, simb->clase, simb->adicional1,
                         simb->adicional2) == ERR) {
      borrar_simbolo(TablaGlobal, id);
      liberar_tabla(TablaLocal);
      TablaLocal = NULL;
      return ERR;
    }
    return OK;
  }
  return ERR;
}

/* Libera la tabla del AmbitoLocal*/
STATUS Liberar_AmbitoLocal() {
  if (!TablaLocal) {
    return ERR;
  }
  liberar_tabla(TablaLocal);
  TablaLocal = NULL;
  return OK;
}

/* Liberamos los dos ambitos, Global y Local*/
void Liberar() {
  if (TablaLocal != NULL) {
    liberar_tabla(TablaLocal);
  }
  if (TablaGlobal != NULL) {
    liberar_tabla(TablaGlobal);
  }
}

/* Funcion que indica si un id en concreto es local, es decir, se encuentra en el ambito local (TablaLocal)*/
int es_Local(const char *id) {

  if (!TablaLocal) {
    return 0;
  }
  return (buscar_simbolo(TablaLocal, id) != NULL);
}
