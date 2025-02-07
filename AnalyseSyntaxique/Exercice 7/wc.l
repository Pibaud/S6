%{
#include <stdio.h>
#include <stdlib.h>

FILE *outputfile;
int lineCount = 0;
int wordCount = 0;
int charCount = 0;
%}

WORD     [a-zA-Z_][a-zA-Z_0-9]*
LINE     [\n]

%%

{WORD}              {wordCount ++; charCount += strlen(yytext);}
{LINE}              {lineCount ++;}

%%

int main(int argc, char **argv) {

    FILE *inputfile = fopen(argv[1], "r");

    yyin = inputfile;

    //sommer la longueur des word

    yylex();

    printf("%d lignes, %d mots, %d char\n", lineCount+1, wordCount, charCount);

    fclose(inputfile);

    return 0;
}