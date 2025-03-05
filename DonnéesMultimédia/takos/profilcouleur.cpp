#include <stdio.h>
#include "image_ppm.h"

/*
Permet Ã  partir d'une image ppm, de crÃ©er un fichier dat, avec la valeur de rouge, vert et bleu de chaque pixel d'une ligne/colone
et d'un indice choisi en paramÃ¨tre

set grid
plot "profilcouleur.dat" using 1:2 with lines title "Rouge" linecolor rgb "red", \
     "profilcouleur.dat" using 1:3 with lines title "Vert" linecolor rgb "green", \
     "profilcouleur.dat" using 1:4 with lines title "Bleu" linecolor rgb "blue"
*/

int main(int argc, char* argv[]) {
    char cNomImgLue[250]; char type[250]; char filename[250];
    int nH, nW, nTaille, nTaille3, indice;

    // VÃ©rification des arg
    if (argc != 5) {
        printf("Usage: ImageIn.ppm filename.dat l||c indice \n");
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", filename);
    sscanf(argv[3], "%s", type);
    sscanf(argv[4], "%d", &indice);

    // Lecture et attributions des dimensions de l'image in
    OCTET *ImgIn;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    // Creation du fichier out
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(1);
    }

    // Ecriture dans le fichier pour chaque pixel de la ligne/colonne, sa valeur de rouge, vert et bleu
    if (strcmp(type, "l") == 0) { // Si de type ligne
        for (int i = 0; i < nW; i++) { // Parcours de chaque pixel de la ligne
            int pixel = 3 * (i + nW * indice);
            fprintf(file, "%d %d %d %d\n", i, ImgIn[pixel], ImgIn[pixel + 1], ImgIn[pixel + 2]);
        }
    } else if (strcmp(type, "c") == 0) { // Si de type colone
        for (int i = 0; i < nH; i++) { // Parcours de chaque pixel de la colone
            int pixel = 3 * (i * nW + indice);
            fprintf(file, "%d %d %d %d\n", i, ImgIn[pixel], ImgIn[pixel + 1], ImgIn[pixel + 2]);
        }
    } else {
        printf("Type != l || c\n");
        exit(1);
    }

    // Return
    fclose(file);
    free(ImgIn);
    return 0;
}
