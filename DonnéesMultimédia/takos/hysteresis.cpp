#include <stdio.h>
#include "image_ppm.h"

/*
Hysteresis d'une image pgm, application de deux doubles boucle 
*/

int main(int argc, char* argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, Sb, Sh;

    // Vérification des arg
    if (argc != 5) {
        printf("Usage: %s ImageIn.pgm ImageOut.pgm SeuilBas Seuilhaut\n", argv[0]);
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &Sb);
    sscanf(argv[3], "%d", &Sh);

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgIn, *ImgMid, *ImgOut;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgMid, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nTaille);

    // Premiere boucle de l'hysteresie
    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            int index = i * nW + j;
            if (ImgIn[index] <= Sb) {
                ImgMid[index] = 0;
            } else if (ImgIn[index] >= Sh) {
                ImgMid[index] = 255;
            } else {
                ImgMid[index] = ImgIn[index];
            }
        }
    }

    // Deuxieme boucle de l'hysteresie
    for (int i = 1; i < nH - 1; i++) {
        for (int j = 1; j < nW - 1; j++) {
            int index = i * nW + j;
            if (Sb < ImgIn[index] && ImgIn[index] < Sh) {
                if ( ImgMid[index - 1] == 255
                    || ImgMid[index + 1]  == 255
                    || ImgMid[index - nW] == 255
                    || ImgMid[index - nW - 1] == 255
                    || ImgMid[index - nW + 1]  == 255
                    || ImgMid[index + nW] == 255
                    || ImgMid[index + nW - 1] == 255
                    || ImgMid[index + nW + 1] == 255) {
                        ImgOut[index] = 0;
                    } else {
                    ImgOut[index] = 255;
                }
            } else {
                ImgOut[index] = ImgMid[index];
            }
        }
    }

    // Écriture de l'image de sortie et return
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
