#include <stdio.h>
#include "image_ppm.h"

/*
Permet à partir d'une image pgm, de créer un fichier dat, avec la valeur en gris de chaque pixel d'une ligne/colone et d'un indice
choisi en paramètre
*/

int main(int argc, char* argv[]) {
    char cNomImgLue[250]; char type[250]; char filename[250];
    int nH, nW, nTaille, indice;

    // Vérification des arg
    if (argc != 5) {
        printf("Usage: ImageIn.pgm filename.dat l||c indice \n");
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", filename);
    sscanf(argv[3], "%s", type);
    sscanf(argv[4], "%d", &indice);

    // Lecture et attributions des dimensions de l'image in
    OCTET *ImgIn;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    // Creation du fichier out
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(1);
    }

    // Ecriture dans le fichier pour chaque pixel de la ligne/colonne, sa valeur en nuance de gris
    if (strcmp(type, "l") == 0) { // Si de type ligne
        for (int i = 0; i < nW; i++) { // Parcours de chaque pixel de la ligne
            int pixel = i + nW * indice;
            fprintf(file, "%d %d\n", i, ImgIn[pixel]);
        }
    } else if (strcmp(type, "c") == 0) { // Si de type colone
        for (int i = 0; i < nH; i++) { // Parcours de chaque pixel de la colone
            int pixel = i * nW + indice;
            fprintf(file, "%d %d\n", i, ImgIn[pixel]);
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
