%{
#include <stdio.h>
#include <ctype.h>

#define DECJ 2
#define HEXAJ 4
#define IDENTIFIERJ 5
#define FLOATJ 6
#define NUMBERJ 7

%}

DEC            [0-9]
NUMBER         [0-9]+
HEXA           0[xX][0-9a-fA-F]+
FLOAT          {DEC}+"."{DEC}*([eE][-+]?{DEC}+)?
IDENTIFIER     [a-zA-Z_][a-zA-Z_0-9]*

%%

{DEC}               { printf("Nombre décimal: %s\n", yytext); return DECJ; }
{NUMBER}            { printf("Nombre décimal: %s\n", yytext); return NUMBERJ; }
{HEXA}              { printf("Hexadecimal: %s\n", yytext); return HEXAJ; }
{FLOAT}             { printf("Floating-point: %s\n", yytext); return FLOATJ; }
{IDENTIFIER}        { printf("Identifier: %s\n", yytext); return IDENTIFIERJ; }
.                   { printf("Caractère ASCII : '%c', code : %d\n", yytext[0], yytext[0]); return yytext[0]; }

%%

int main() {
    while (yylex()) {
        printf("Lexème: %s\n", yytext);
    }
    return 0;
}

int yywrap() {
    return 1;
}