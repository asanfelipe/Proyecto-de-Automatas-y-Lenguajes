#include "generacion.h"
#include <stdlib.h>
#include <string.h>


/**
  Codigo para el principio de la seccion .bss
  Guardamos el puntero de pila extendido en (esp)
*/
void escribir_cabecera_bss(FILE *fpasm) {

  fprintf(fpasm, "segment .bss\n");
  fprintf(fpasm, "\t__esp resd 1\n");
}

/**
  Escribimos el princpio de la subseccion .data
  Declaramos con db, texto de los errores.
    Indice fuera de rango
    Error division por 0
*/
void escribir_subseccion_data(FILE *fpasm) {

  fprintf(fpasm, "segment .data\n");
  fprintf(fpasm, "\t_errorrango db \"INDICE FUERA DE RANGO\", 0\n");
  fprintf(fpasm, "\t_errordivision db \"ERROR DIVISION POR 0\", 0\n");
  fprintf(fpasm, "\n");
}

/**
  Esta funcion sirve para declaracion de variables en la seccion .bss.
  nombre: nombre de la variables
  tipo: ENTERO o BOOLEANO (mira en .h)
  tamano: en la P1 siempre=1. Este tamano es para la declaracion de vectores
*/
void declarar_variable(FILE *fpasm, char *nombre, int tipo, int tamano) {

  fprintf(fpasm, "\t_%s resd %d\n", nombre, tamano);
  fprintf(fpasm, "\n");
}

/**
  Codigo para la secciond de codigo .text
  Anadimos una libreria con funcoines de apoyo a la generacion de codigo (diapos
  9)
*/
void escribir_segmento_codigo(FILE *fpasm) {

  fprintf(fpasm, "segment .text\n");
  fprintf(fpasm, "\tglobal main\n");
  fprintf(fpasm, "\textern scan_int, scan_boolean\n");
  fprintf(fpasm, "\textern print_int, print_boolean, print_string, "
                 "print_blank, print_endofline\n");
  fprintf(fpasm, "\n");
}

/**
  Escribimos la etiqueta main (diapos 12)
  Guardamos el puntero de pila en su variable usando __esp
*/
void escribir_inicio_main(FILE *fpasm) {

  fprintf(fpasm, "\tmain:\n");
  fprintf(fpasm, "\tmov dword [__esp], esp\n");
}

/**
  Escribe el codigo para salir de manera controlada del programa,
  en caso de que el programa haya finalizado correctamente,
  o que se haya producido alguno de los dos errores declarados anteriormente.
  (codigo en diapos avanzadas)
*/
void escribir_fin(FILE *fpasm) {

  /* Saltamos al exit */
  fprintf(fpasm, "jmp _exit\n");

  /* Controlamos el error producido por un indice fuera de rango*/
  fprintf(fpasm, "_errrango: \n");
  fprintf(fpasm, "\tpush dword _errorrango\n");
  fprintf(fpasm, "\tcall print_string\n");
  fprintf(fpasm, "\tadd esp, 4\n");
  fprintf(fpasm, "\tcall print_endofline\n");
  fprintf(fpasm, "\tjmp _exit\n");

  /*Controlamos el error producido por una division por cero*/
  fprintf(fpasm, "_errdivision:\n");
  fprintf(fpasm, "\tpush dword _errordivision\n");
  fprintf(fpasm, "\tcall print_string\n");
  fprintf(fpasm, "\tadd esp, 4\n");
  fprintf(fpasm, "\tcall print_endofline\n");
  fprintf(fpasm, "\tjmp _exit\n");

  /* Salimos del programa de manera controlada */
  fprintf(fpasm, "_exit: \n");
  fprintf(fpasm, "\tmov dword esp, [__esp]\n");
  fprintf(fpasm, "\tret\n");

  fclose(fpasm);
}

/**
  Al saber un operando, esta funcion lo mete en la pila.
  nombre: cadena de caracteres del operando (aparacen como aparecen en NASM)
  es_variable: si es = 0, no es variable, es decir, es un num directamente
               si es = 1, es una variable. (Por ejemplo b1)
*/
void escribir_operando(FILE *fpasm, char *nombre, int es_variable) {

  /* Tenemos que hacer un un if para contemplar las dos opciones,
      ya que se representan con un _ delante
      dependiendo de si es o no variable
  */
  if (es_variable == 1)
    fprintf(fpasm, "\tpush dword _%s\n", nombre);
  else
    fprintf(fpasm, "\tpush dword %s\n", nombre);
}

/**
  Asignamos valor a la variable de nombre nombre.
  Primero cogemos el valor de la cima de la pila,
  para despues poder asignarle el valor de la variable nombre
*/
void asignar(FILE *fpasm, char *nombre, int es_variable) {

  if (es_variable == 1) {
    fprintf(fpasm, "\tpop dword eax\n");
    fprintf(fpasm, "\tmov dword [_%s], eax\n", nombre);
  } else {
    fprintf(fpasm, "\tpop dword eax\n");
    fprintf(fpasm, "\tmov dword [_%s], eax\n", nombre);
  }
}

void sumar(FILE *fpasm, int es_variable_1, int es_variable_2) {
  /*Los dos últimos argumentos indican respectivamente si lo que hay en la pila
    es una referencia a un valor o un valor explícito.*/
  /*Primero extraemos los operandos de la pila*/
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable_1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable_2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  /*Realizamos la operacion*/
  fprintf(fpasm, "\tadd eax, ebx\n");
  /*Guardamos los datos en la pila*/
  fprintf(fpasm, "\tpush dword eax\n");
}

void restar(FILE *fpasm, int es_variable_1, int es_variable_2) {
  /*Primero extraemos los operandos de la pila*/
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable_1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable_2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  /*Realizamos la operacion*/
  fprintf(fpasm, "\tsub eax, ebx\n");
  /*Guardamos los datos en la pila*/
  fprintf(fpasm, "\tpush dword eax\n");
}


void multiplicar(FILE *fpasm, int es_variable_1, int es_variable_2) {
  /*Primero extraemos los operandos de la pila*/
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable_1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable_2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  /*Realizamos la operacion*/
  fprintf(fpasm, "\timul ebx\n");
  /*Guardamos los datos en la pila*/
  fprintf(fpasm, "\tpush dword eax\n");
}


void dividir(FILE *fpasm, int es_variable_1, int es_variable_2) {
  /*Primero extraemos los operandos de la pila*/
  fprintf(fpasm, "\tpop dword ecx\n"); /* cargar el primer operando en ecx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable_1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable_2 == 1)
    fprintf(fpasm, "\tmov dword ecx, [ecx]\n");

  /*Comprobamos la division por cero (divisor > 0)*/
  fprintf(fpasm, "\tcmp ecx, 0\n");
  fprintf(fpasm,
          "\tje _errdivision\n"); /* salto si igual a la etiqueta _errdivison */
  fprintf(fpasm, "\tcdq\n");      /* Extender el dividendo a la composición de
                                     registros edx:eax (cdq) */
  fprintf(fpasm, "\tidiv ecx\n");
  fprintf(
      fpasm,
      "\tpush dword eax\n"); /* el resultado de la division se guarda en eax */
}


void o(FILE *fpasm, int es_variable_1, int es_variable_2) {
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable_1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable_2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  /*realizamos la conjuncion y dejamos el resultado en eax*/
  fprintf(fpasm, "\tor eax, ebx\n");

  /*apilamos es resultado*/
  fprintf(fpasm, "\tpush dword eax\n");
}


void y(FILE *fpasm, int es_variable_1, int es_variable_2) {
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable_1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable_2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  /*realizamos la conjuncion y dejamos el resultado en eax*/
  fprintf(fpasm, "\tand eax, ebx\n");

  /*apilamos es resultado*/
  fprintf(fpasm, "\tpush dword eax\n");
}


void cambiar_signo(FILE *fpasm, int es_variable) {
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */

  if (es_variable == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  fprintf(fpasm, "\timul eax, -1\n");
  fprintf(fpasm, "\tpush eax\n");
}


void no(FILE *fpasm, int es_variable, int cuantos_no) {
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */

  if (es_variable == 1) {
    fprintf(fpasm, "\tmov dword eax, [eax]\n");
  }
  /* si encuentra en la cima de la pila un 0 deja en la cima un 1 y al
   * contrario.*/
  fprintf(fpasm, "\tcmp eax, 0\n");
  fprintf(fpasm, "\tje _etiqueta%d\n",
          cuantos_no); /* si la cima es un cero salta */
  fprintf(fpasm, "\tpush dword 0\n");
  fprintf(fpasm, "\tjmp _end%d\n", cuantos_no); /* si la cima es un uno salta */
  fprintf(fpasm, "_etiqueta%d: \n", cuantos_no);
  fprintf(fpasm, "\tpush dword 1\n");
  fprintf(fpasm, "_end%d: \n", cuantos_no);
}


void igual(FILE *fpasm, int es_variable1, int es_variable2, int etiqueta) {
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  fprintf(fpasm, "\tcmp eax, ebx\n"); /* comparamos las dos variables */
  fprintf(fpasm, "\tje _igual%d\n",
          etiqueta); /* saltamos a la etiqueta si son iguales */
  fprintf(fpasm, "\tpush dword 0\n"); /* apilamos 0 si no se cumple */
  fprintf(fpasm, "\tjmp _fin_igual%d\n", etiqueta); /* salimos de la funcion */
  fprintf(fpasm, "_igual%d:\n", etiqueta);          /* etiqueta igual */
  fprintf(fpasm,
          "\tpush dword 1\n"); /* pusheamos 1 si se cumple la condición */
  fprintf(fpasm, "_fin_igual%d:\n", etiqueta); /* etiqueta fin_igual */
}


void distinto(FILE *fpasm, int es_variable1, int es_variable2, int etiqueta) {
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  fprintf(fpasm, "\tcmp eax, ebx\n"); /* comparamos las dos variables */
  fprintf(fpasm, "\tjne _distinto%d\n",
          etiqueta); /* saltamos a la etiqueta si no son iguales */
  fprintf(fpasm, "\tpush dword 0\n"); /* apilamos 0 si no se cumple */
  fprintf(fpasm, "\tjmp _fin_distinto%d\n",
          etiqueta);                          /* salimos de la funcion */
  fprintf(fpasm, "_distinto%d:\n", etiqueta); /* etiqueta distinto */
  fprintf(fpasm,
          "\tpush dword 1\n"); /* pusheamos 1 si se cumple la condición */
  fprintf(fpasm, "_fin_distinto%d:\n", etiqueta); /* etiqueta fin_distinto */
}


void menor_igual(FILE *fpasm, int es_variable1, int es_variable2,
                 int etiqueta) {

  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  fprintf(fpasm, "\tcmp eax, ebx\n"); /* comparamos las dos variables */
  fprintf(fpasm, "\tjle _menoroigual%d\n",
          etiqueta); /* saltamos a la etiqueta si es menor o igual */
  fprintf(fpasm, "\tpush dword 0\n"); /* apilamos 0 si no se cumple */
  fprintf(fpasm, "\tjmp _fin_menoroigual%d\n",
          etiqueta);                             /* salimos de la funcion */
  fprintf(fpasm, "_menoroigual%d:\n", etiqueta); /* etiqueta menoroigual */
  fprintf(fpasm,
          "\tpush dword 1\n"); /* pusheamos 1 si se cumple la condición */
  fprintf(fpasm, "_fin_menoroigual%d:\n",
          etiqueta); /* etiqueta fin_menoroigual */
}



void mayor_igual(FILE *fpasm, int es_variable1, int es_variable2,
                 int etiqueta) {
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  fprintf(fpasm, "\tcmp eax, ebx\n"); /* comparamos las dos variables */
  fprintf(fpasm, "\tjge _mayoroigual%d\n",
          etiqueta); /* saltamos a la etiqueta si es mayor o igual */
  fprintf(fpasm, "\tpush dword 0\n"); /* apilamos 0 si no se cumple */
  fprintf(fpasm, "\tjmp _fin_mayoroigual%d\n",
          etiqueta);                             /* salimos de la funcion */
  fprintf(fpasm, "_mayoroigual%d:\n", etiqueta); /* etiqueta mayoroigual */
  fprintf(fpasm,
          "\tpush dword 1\n"); /* pusheamos 1 si se cumple la condición */
  fprintf(fpasm, "_fin_mayoroigual%d:\n",
          etiqueta); /* etiqueta fin_mayoroigual */
}


void menor(FILE *fpasm, int es_variable1, int es_variable2, int etiqueta) {
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */
  if (es_variable1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  fprintf(fpasm, "\tcmp eax, ebx\n"); /* comparamos las dos variables */
  fprintf(fpasm, "\tjl _menor%d\n",
          etiqueta);                  /* saltamos a la etiqueta si es menor */
  fprintf(fpasm, "\tpush dword 0\n"); /* apilamos 0 si no se cumple */
  fprintf(fpasm, "\tjmp _fin_menor%d\n", etiqueta); /* salimos de la funcion */
  fprintf(fpasm, "_menor%d:\n", etiqueta);          /* etiqueta menoroigual */
  fprintf(fpasm,
          "\tpush dword 1\n"); /* pusheamos 1 si se cumple la condición */
  fprintf(fpasm, "_fin_menor%d:\n", etiqueta); /* etiqueta fin_menor */
}

void mayor(FILE *fpasm, int es_variable1, int es_variable2, int etiqueta) {
  fprintf(fpasm, "\tpop dword ebx\n"); /* cargar el primer operando en ebx */
  fprintf(fpasm, "\tpop dword eax\n"); /* cargar el segundo operando en eax */

  if (es_variable1 == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  if (es_variable2 == 1)
    fprintf(fpasm, "\tmov dword ebx, [ebx]\n");

  fprintf(fpasm, "\tcmp eax, ebx\n"); /* comparamos las dos variables */
  fprintf(fpasm, "\tjg _mayor%d\n",
          etiqueta);                  /* saltamos a la etiqueta si es mayor */
  fprintf(fpasm, "\tpush dword 0\n"); /* apilamos 0 si no se cumple */
  fprintf(fpasm, "\tjmp _fin_mayor%d\n", etiqueta); /* salimos de la funcion */
  fprintf(fpasm, "_mayor%d:\n", etiqueta);          /* etiqueta mayoroigual */
  fprintf(fpasm,
          "\tpush dword 1\n"); /* pusheamos 1 si se cumple la condición */
  fprintf(fpasm, "_fin_mayor%d:\n", etiqueta); /* etiqueta fin_mayor */
}


void leer(FILE *fpasm, char *nombre, int tipo) {

  /* Guardamos en la pila el nombre de la variable a leer*/
  fprintf(fpasm, "\tpush dword _%s\n", nombre);
  /* Lee  ENTERO*/
  if (tipo == ENTERO) {
    fprintf(fpasm, "\tcall scan_int\n");
  }
  /* Lee  BOOLEAN*/
  else if (tipo == BOOLEANO) {
    fprintf(fpasm, "\tcall scan_boolean\n");
  }
  /*Recuperamos el valor de la pila*/
  fprintf(fpasm, "\tadd esp, 4\n");
}


void escribir(FILE *fpasm, int es_variable, int tipo) {
  /*Si es 0 entonces se ha pasado por valor en la pila
  si no, se ha pasado la direccion del registro*/
  if (es_variable == 1) {
    /*Cargar contenido */
    fprintf(fpasm, "\tpop dword eax\n");
    fprintf(fpasm, "\tmov dword eax, [eax]\n");
    /* Asignar a "nombre" el valor leido de la pila */
    fprintf(fpasm, "\tpush dword eax\n");
  }

  /* Lee ENTERO*/
  if (tipo == ENTERO) {
    fprintf(fpasm, "\tcall print_int\n");
  }
  /* Lee BOOLEAN*/
  else if (tipo == BOOLEANO) {
    fprintf(fpasm, "\tcall print_boolean\n");
  }

  fprintf(fpasm, "\tadd esp, 4\n");
  fprintf(fpasm, "\tcall print_endofline\n");
}


void ifthenelse_inicio(FILE *fpasm, int exp_es_variable, int etiqueta) {

  /* Extraemos de la pila la expresión*/
  fprintf(fpasm, "\tpop eax\n");
  /* Comprobamos si es variable*/
  if (exp_es_variable == 1) {
    fprintf(fpasm, "\tmov dword eax, [eax]\n");
  }

  /* Si la expresion es 1, se cumple la condición del IF*/
  fprintf(fpasm, "\tcmp eax, 1\n");
  /* Si la expresion NO es 1, comienza el else*/
  fprintf(fpasm, "\tjne fin_then_%d\n", etiqueta);
}


void ifthen_inicio(FILE *fpasm, int exp_es_variable, int etiqueta) {

  /* Extraemos de la pila la expresión*/
  fprintf(fpasm, "\tpop eax\n");
  /* Comprobamos si es variable*/
  if (exp_es_variable == 1) {
    fprintf(fpasm, "\tmov dword eax, [eax]\n");
  }

  /* Si la expresion es 1, se cumple la condición del IF*/
  fprintf(fpasm, "\tcmp eax, 1\n");
  /* Si la expresion NO es 1, NO entra en el if, salta al then*/
  fprintf(fpasm, "\tjne fin_then_%d\n", etiqueta);
}


void ifthen_fin(FILE *fpasm, int etiqueta) {

  fprintf(fpasm, "fin_then_%d:\n", etiqueta);
}


void ifthenelse_fin_then(FILE *fpasm, int etiqueta) {

  /* Ha terminado el bloque THEN, salta el bloque ELSE y va al final */
  fprintf(fpasm, "\tjmp fin_then_else_%d\n", etiqueta);
  /*  Etiqueta del fin del bloque */
  fprintf(fpasm, "fin_then_%d:\n", etiqueta);
}


void ifthenelse_fin(FILE *fpasm, int etiqueta) {

  fprintf(fpasm, "fin_then_else_%d:\n", etiqueta);
}


void while_inicio(FILE *fpasm, int etiqueta) {

  fprintf(fpasm, "_inicio_while_%d:\n", etiqueta);
}


void while_exp_pila(FILE *fpasm, int exp_es_variable, int etiqueta) {
  /* Se saca de la cima de la pila el valor de la expresión que gobierna el
   * bucle*/
  fprintf(fpasm, "pop dword eax\n");

  if (exp_es_variable > 0)
    fprintf(fpasm, "mov dword eax, [eax]\n");

  /*si eax es 0 se salta al final del bucle, habriamos terminado*/
  fprintf(fpasm, "cmp eax, 0\n");
  fprintf(fpasm, "je _fin_while_%d\n", etiqueta);
}


void while_fin(FILE *fpasm, int etiqueta) {
  /*Salta al nuevo principio del bucle para poder evaluar la condición de
   * salida*/
  fprintf(fpasm, "jmp _inicio_while_%d\n", etiqueta);
  /*Se escribe la etiqueta de fin de bucle*/
  fprintf(fpasm, "_fin_while_%d:\n", etiqueta);
}


void escribir_elemento_vector(FILE *fpasm, char *nombre_vector, int tam_max,
                              int exp_es_direccion) {
  /*Se saca de la pila a un registro el valor de un indice*/
  fprintf(fpasm, "pop dword eax\n");

  /*haciendo lo que proceda en el caso de que sea una dirección (variable o
   * equivalente)*/
  if (exp_es_direccion == 1)
    fprintf(fpasm, "mov dword eax, [eax]\n");

  /*se programa el control de errores en tiempo de ejecucion
    si es indice es < 0 se termina el programa si no continua
  */
  fprintf(fpasm, "cmp eax, 0\n");
  /*se supone que en la direccion fin_indice_fuera_rango se procesa este error
   * en tiempo de ejecucion*/
  fprintf(fpasm, "jl _errrango\n");
  /*Si el indice es > maximo permitido se termina el programa, si no, continua*/
  fprintf(fpasm, "cmp eax, %d\n", tam_max - 1);
  fprintf(fpasm, "jg _errrango\n");
  fprintf(fpasm, "mov dword edx, _%s\n", nombre_vector);
  /*direccion elemento indexado en eax*/
  fprintf(fpasm, "lea eax, [edx + eax*4]\n");
  /*direccion del elemento indexado en la cima de la pila*/
  fprintf(fpasm, "push dword eax\n");
}


void declararFuncion(FILE *fd_asm, char *nombre_funcion, int num_var_loc) {
  /*etiqueta de inicio de la funcion*/
  fprintf(fd_asm, "_%s:\n", nombre_funcion);
  /*preservacion de ebp/esp*/
  fprintf(fd_asm, "push ebp\n");
  fprintf(fd_asm, "mov ebp, esp\n");
  /*reserva de espacio para las variables locales de la pila*/
  fprintf(fd_asm, "sub esp, %d\n", 4 * num_var_loc);
}


void retornarFuncion(FILE *fd_asm, int es_variable) {
  fprintf(fd_asm, "\t pop eax\n");
  if (es_variable == 1)
    fprintf(fd_asm, "\tmov dword eax, [eax]\n");

  /*restaurar el puntero de la pila*/
  fprintf(fd_asm, "\tmov esp, ebp\n");
  /*sacar de la pila ebp*/
  fprintf(fd_asm, "pop ebp\n");
  /*vuelve al programa llamante y saca de la pila la direccion de retorno*/
  fprintf(fd_asm, "\tret\n");
}


void escribirParametro(FILE *fpasm, int pos_parametro,
                       int num_total_parametros) {
  int d_edp = 4 * (1 + (num_total_parametros - pos_parametro));

  fprintf(fpasm, "lea eax, [ebp + %d]\n", d_edp);
  fprintf(fpasm, "push dword eax\n");
}


void escribirVariableLocal(FILE *fpasm, int posicion_variable_local) {

  int d_ebp;

  d_ebp = 4 * posicion_variable_local;

  fprintf(fpasm, "pop dword eax\n");
  fprintf(fpasm, "mov dword [ebp+%d], eax\n", d_ebp);
}


void asignarDestinoEnPila(FILE *fpasm, int es_variable) {

  /* Tomamos la direccino donde vamos a asignar*/
  fprintf(fpasm, "\tpop dword eax\n");

  /* Tomamos el valor que se debe asignar incluso desreferenciando en el caso de
   * que sea una variable.
   * Esto se hace ya que si es_variable==1, es una direccion/nombre de variable
   * por lo que no lo podemos asignar directamente
   */
  fprintf(fpasm, "\tpop dword ebx\n");
  if (es_variable == 1)
    fprintf(fpasm, "\tmov dword eax, [eax]\n");

  /* Por ultimo, asignamos */
  fprintf(fpasm, "\tmov dword [ebx], eax\n");
}


void operandoEnPilaAArgumento(FILE *fd_asm, int es_variable) {

  /* En el caso de que en la pila tengamos una variable y no un valor */
  if (es_variable == 1) {
    fprintf(fd_asm, "\tpop dword eax\n");
    /* Accedemos a nuestra variable, cogemos su valor y lo volvemos a introducir
     * en la pila
     */
    fprintf(fd_asm, "\tmov dword eax, [eax]\n");
    fprintf(fd_asm, "\tpush dword eax\n");
  }
}


void llamarFuncion(FILE *fd_asm, char *nombre_funcion, int num_argumentos) {

  /* Llamamos a la funcion */
  fprintf(fd_asm, "\tcall _%s\n", nombre_funcion);

  /* Limpiamos los argumentos usados en la llamada a la funcion almacenados en
   * la pila
   */
  fprintf(fd_asm, "\tadd esp, %d\n", 4 * num_argumentos);

  /* Dejamos en la cima de la pila el retorno de la funcions despues de haberla
   * limpiado de sus argumentos
   */
  fprintf(fd_asm, "\tpush dword eax\n");
}


/* Limpia la pila tras la llamada a una funcion */
void limpiarPila(FILE *fd_asm, int num_argumentos) {

  fprintf(fd_asm, "\tadd esp, %d*4\n", num_argumentos);
}

/* Insertar el parametro si su categoría es PARAMETRO */
void parametroFuncion(FILE* fpasm, int pos_parametro) {
    fprintf(fpasm, "mov dword eax, ebp\n");
    fprintf(fpasm, "add eax, %d\n", 4*pos_parametro);
    fprintf(fpasm, "push dword eax\n");
}
