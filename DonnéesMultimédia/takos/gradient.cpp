#include <stdio.h>
#include <math.h>
#include "image_ppm.h"

/*
Caclul du gradient d'une image pgm, pour chaque pixel, on calcul son gradient
*/

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;

    // Vérification des arg
    if (argc != 3) {
        printf("Usage: ImageIn.pgm ImageOut.pgm \n");
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    // Lecture et attributions des dimensions de l'image in1/in2/out
    OCTET *ImgIn, *ImgOut;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // Parcours de chaque pixel de l'image
    for (int i = 1; i < nH - 1; i++) {
        for (int j = 1; j < nW - 1; j++) {
            int index = i * nW + j;
            int horizontal = ImgIn[index + 1] - ImgIn[index];
            int vertical = ImgIn[index + nW] - ImgIn[index];
            unsigned char gradient = sqrt(horizontal*horizontal + vertical*vertical); // Calcul du gradient
            ImgOut[index] = gradient;
        }
    }

    // Ecriture dans l'image de sortie et return
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}