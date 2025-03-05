#include <stdio.h>
#include "image_ppm.h"

/* 
Le programme permet de seuiller une image pgm avec trois seuils en paramètres, ce qui permet de faire ressortir les pixels
sous le premier seuil de valeurs en noir, ceux entre le premier et deuxième seuil en gris clair, ceux entre le deuxième et
le troisième seuil en gris foncé, et ceux au dessus du 3ème seuil en blanc
*/

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, S1, S2, S3;

    // Vérification des arg
    if (argc != 6) {
        printf("Usage: ImageIn.pgm ImageOut.pgm Seuil1 Seuil2 Seuil3 \n");
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &S1);
    sscanf(argv[4], "%d", &S2);
    sscanf(argv[5], "%d", &S3);

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgIn, *ImgOut;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // Parcours de chaque pixel de l'image
    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            if (ImgIn[i * nW + j] < S1) {
                ImgOut[i * nW + j] = 0;
            } else if (ImgIn[i * nW + j] < S2) {
                ImgOut[i * nW + j] = 85;
            } else if (ImgIn[i * nW + j] < S3) {
                ImgOut[i * nW + j] = 170;
            } else {
                ImgOut[i * nW + j] = 255;
            }
        }
    }

    // Écriture de l'image de sortie et return
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
