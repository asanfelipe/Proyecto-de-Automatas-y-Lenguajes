#include "alfa.h"
#include "y.tab.h"
#include "generacion.h"


extern FILE* yyin;
extern FILE* yyout;


int main(int argc, char** argv) {
  if (argc == 2) {
    yyin = fopen(argv[1], "r");
    yyout = stdout;
  } else if (argc == 3) {
    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");
  } else {
    fprintf(stderr, "formato incorrecto:\n./alfa input output\n");
    return 1;
  }

    escribir_subseccion_data(yyout);
    escribir_cabecera_bss(yyout);

    if(yyparse()!=0){
    } else {
        escribir_fin(yyout);
    }

  fclose(yyin);

  return 0;
}