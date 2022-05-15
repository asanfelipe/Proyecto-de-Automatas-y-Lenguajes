%{

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "hash_table.h"
#include "generacion.h"
#include "symbols_table.h"
#include "alfa.h"

#include "y.tab.h"

#define ERROR_MORFO stderr

extern FILE* yyin;
extern FILE* yyout;

int columna = 0;
int fila = 1;
int eror = 0;
%}

 
%option noyywrap

DIGITO          [0-9]
LETRA           [a-zA-Z]
ENTERO          {DIGITO}*
IDENTIFICADOR   {LETRA}({LETRA}|{DIGITO})*


%%




"//".* { columna = 0; }


\n {
    columna = 0;
    fila +=1;
}

[ \t] {    columna +=1;}

\r {    }



main {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_MAIN;}

int {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_INT;}

boolean {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_BOOLEAN;}

array {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_ARRAY;}

"function" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_FUNCTION;}

if {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_IF;}

else {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_ELSE;}

while {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_WHILE;}

scanf {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_SCANF;}

printf {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_PRINTF;}

return {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_RETURN;}

";" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_PUNTOYCOMA;}

"," {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_COMA;}

"(" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_PARENTESISIZQUIERDO;}

")" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_PARENTESISDERECHO;}

"[" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_CORCHETEIZQUIERDO;}

"]" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_CORCHETEDERECHO;}

"{" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_LLAVEIZQUIERDA;}

"}" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_LLAVEDERECHA;}

"=" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_ASIGNACION;}

"+" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_MAS;}

"-" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_MENOS;}

"/" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_DIVISION;}

"*" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_ASTERISCO;}
"&&" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_AND;}
"||" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_OR;}

"!=" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_DISTINTO;}

"!" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_NOT;}

"==" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_IGUAL;}

"<=" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_MENORIGUAL;}

">=" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_MAYORIGUAL;}

"<" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_MENOR;}

">" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_MAYOR;}


"true" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_TRUE;}

"false" {    fprintf(yyout,";D: %s\n",yytext); columna += yyleng; return TOK_FALSE;}

{ENTERO}    {
                        yylval.atributos.tipo = ENTERO;
                        yylval.atributos.valor_entero = atoi(yytext);
                        strcpy(yylval.atributos.lexema , yytext);
                        columna+=yyleng; fprintf(yyout, ";D:\t%s\n", yytext); return TOK_CONSTANTE_ENTERA;
                    }

{IDENTIFICADOR}     {
                        if (yyleng > 100) { 
                            fprintf(ERROR_MORFO,"****Error en [fila %d, columna %d]: identificador demasiado largo (%s)\n",fila,columna,yytext);
                            eror=1;
                            return TOK_ERROR;
                        } else {
                            columna += yyleng;
                                strcpy(yylval.atributos.lexema , yytext);
                            fprintf(yyout, ";D:\t%s\n", yytext);
                            return TOK_IDENTIFICADOR;
                        }
                    }


. {
    columna += yyleng;
    fprintf(ERROR_MORFO,"**Error en [fila %d, columna %d]: simbolo no permitido (%s)\n",fila,columna,yytext);
    eror=1;
    return TOK_ERROR;
}

%%