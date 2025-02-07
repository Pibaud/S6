%{
#include <stdio.h>
#include <stdlib.h>

FILE *outputfile;
%}

%%

^[ \t]*$\n(^(?![ \t]*$).+\n)+^[ \t]*$    {fprintf(outpufile, "<ul>"++"</ul>")} // déplacer le pointeur après écriture on a  : DEBUTFICHIERlignevide<ul></ul>lignevideEOF
^[*]                                     {fprintf(outpufile, "<ul><li>A</li></ul>")} // début des items de la liste de 1er niveau
^[ \t]+                                  {}   // Supprime les espaces en début de ligne
[ \t]+$                                  {}   // Supprime les espaces en fin de ligne
[ \t]+                                   {fprintf(outputfile, " ");}  // Remplace plusieurs espaces/tabulations par un seul espace
.                                        {fprintf(outputfile, "%s", yytext);} // Écrit tout autre caractère dans le fichier

%%

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    outputfile = fopen("output.html", "w");

    FILE *inputfile = fopen(argv[1], "r");

    yyin = inputfile;

    yylex();

    fclose(inputfile);
    fclose(outputfile);

    return 0;
}