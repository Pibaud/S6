#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, nR, nG, nB, Sr, Sg, Sb;

    // Vérification des arg
    if (argc != 6) {
        printf("Usage: ImageIn.ppm ImageOut.ppm SeuilR SeuilG SeuilB \n");
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &Sr);
    sscanf(argv[4], "%d", &Sg);
    sscanf(argv[5], "%d", &Sb);

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgIn, *ImgOut;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);

    // Application du seuil à l'image out
    for (int i = 0; i < nTaille3; i += 3) {
        nR = ImgIn[i]; // Rouge
        nG = ImgIn[i + 1]; // Vert
        nB = ImgIn[i + 2]; // Bleu
        if (nR < Sr) {
            ImgOut[i] = 0;
        } else {
            ImgOut[i] = 255;
        }
        if (nG < Sg) {
            ImgOut[i+1] = 0;
        } else {
            ImgOut[i+1] = 255;
        }
        if (nB < Sb) {
            ImgOut[i+2] = 0;
        } else {
            ImgOut[i+2] = 255;
        }
    }

    // Écriture de l'image de sortie et return
    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
