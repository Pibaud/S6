#include <stdio.h>
#include "image_ppm.h"

/*
Conversion d'une image ppm en image pgm, pour chaque pixel, on fait la moyenne entre la valeur de rouge, vert et bleu
*/

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, nR, nG, nB;

    // Vérification des arg
    if (argc != 3) {
        printf("Usage: %s ImageIn.ppm ImageOut.pgm\n", argv[0]);
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgIn, *ImgOut;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    allocation_tableau(ImgOut, OCTET, nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    // Parcours de chaque pixel de l'image
    for (int i = 0; i < nTaille3; i += 3) {
        nR = ImgIn[i]; // Rouge
        nG = ImgIn[i + 1]; // Vert
        nB = ImgIn[i + 2]; // Bleu

        int res = (nR + nG + nB) / 3; // Moyenne des valeurs des pixels de couleurs
        ImgOut[i / 3] = res;
    }

    // Écriture de l'image de sortie et return
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
