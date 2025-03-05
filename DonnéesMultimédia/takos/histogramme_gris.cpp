#include <stdio.h>
#include "image_ppm.h"

/*
Histogramme : Notation dans un fichier dat de chaque occurences de valeur de gris de l'image pgm
*/

int main(int argc, char* argv[]) {
    char cNomImgLue[250]; char filename[250];
    int nH, nW, nTaille;
    int histo[256] = {0};

    // Vérification des arg
    if (argc != 3) {
        printf("Usage: ImageIn.pgm filename.dat \n");
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", filename);

    // Lecture et attributions des dimensions de l'image in
    OCTET *ImgIn, *ImgOut;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // Définition des occurences de nuance de gris de chaque pixel dans histo[], histo[0...255] = nbr occurences
    for (int i = 0; i < nTaille; i++) {
        histo[ImgIn[i]]++;
    }

    // Creation du fichier out
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(1);
    }

    // Ecriture dans le fichier pour chaque pixel, son nombre d'occurences a partir de histo
    int nbrPixels = 0;
    for (int i = 0; i < 256; i++) {
        fprintf(file, "%d %d\n", i, histo[i]);
        // printf("Indice : %d - Occurences : %d\n", i, histo[i]);
        nbrPixels += histo[i];
    }
    // printf("Nombre total de pixels : %d\n", nbrPixels);

    // Return
    fclose(file);
    free(ImgIn);
    return 0;
}
