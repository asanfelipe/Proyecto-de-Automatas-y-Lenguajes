%{
#include <stdio.h>
#include "tokens.h"
#define TOK_SALTO_LINEA 500
%}

%option noyywrap

%%
 /*PALABRAS RESERVADAS*/
main { return TOK_MAIN; }
int { return TOK_INT; }
boolean { return TOK_BOOLEAN; }
array { return TOK_ARRAY; }
function { return TOK_FUNCTION; }
if { return TOK_IF; }
else { return TOK_ELSE; }
while { return TOK_WHILE; }
scanf { return TOK_SCANF; }
printf { return TOK_PRINTF; }
return { return TOK_RETURN; }
 /*SIMBOLOS*/
; { return TOK_PUNTOYCOMA; }
, { return TOK_COMA; }
[(] { return TOK_PARENTESISIZQUIERDO; }
[)] { return TOK_PARENTESISDERECHO; }
[[] { return TOK_CORCHETEIZQUIERDO; }
[]] { return TOK_CORCHETEDERECHO; }
[{] { return TOK_LLAVEIZQUIERDA; }
[}] { return TOK_LLAVEDERECHA; }
[=] { return TOK_ASIGNACION; }
[+] { return TOK_MAS; }
[-] { return TOK_MENOS; }
[/] { return TOK_DIVISION; }
[*] { return TOK_ASTERISCO; }
&& { return TOK_AND; }
[||] { return TOK_OR; }
[!] { return TOK_NOT; }
"==" { return TOK_IGUAL; }
"!=" { return TOK_DISTINTO; }
"<=" { return TOK_MENORIGUAL; }
">=" { return TOK_MAYORIGUAL; }
[<] { return TOK_MENOR; }
[>] { return TOK_MAYOR; }
 /*CONSTANTES*/
[0-9]+ { return TOK_CONSTANTE_ENTERA; }
TRUE { return TOK_TRUE; }
FALSE { return TOK_FALSE; }
 /*SALTO DE LINEA*/
[\r\t] { return TOK_SALTO_LINEA; }
 /*COMENTARIOS*/
"//"[^\n]*
 /*ESPACIO EN BLANCO*/
[" "\t]+
 /*ERRORES*/
[a-zA-Z0-9]{100,} { return TOK_ERROR; }
 /*IDENTIFICADORES*/
[a-zA-Z][a-zA-Z0-9]* { return TOK_IDENTIFICADOR; }
%%