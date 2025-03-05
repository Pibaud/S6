#include <stdio.h>
#include "image_ppm.h"

/* 
Le programme permet de seuiller une image pgm avec un seuil en paramètre, ce qui permet de faire ressortir les pixels
sous un certain seuil de valeurs en noir, et ceux au dessus en blanc
*/

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, S;

    // Vérification des arg
    if (argc != 4) {
        printf("Usage: %s ImageIn.pgm ImageOut.pgm Seuil\n", argv[0]);
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &S);

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgIn, *ImgOut;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nTaille);

    // Application du seuil à l'image out
    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            int index = i * nW + j;
            ImgOut[index] = (ImgIn[index] < S) ? 0 : 255;
        }
    }

    // Écriture de l'image de sortie et return
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
