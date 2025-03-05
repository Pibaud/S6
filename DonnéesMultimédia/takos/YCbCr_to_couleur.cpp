#include <stdio.h>
#include "image_ppm.h"

/*
Conversion de trois images pgm Y, Cb, Cr en image ppm, pour chaque pixel, on reconstruit la valeur de rouge, vert et bleu
avec des formules mathematiques
*/

int main(int argc, char *argv[]) {
    char cNomImgLueY[250], cNomImgLueCb[250], cNomImgLueCr[250], cNomImgEcrite[250];
    int nH, nW, nTaille;

    // Vérification des arg
    if (argc != 5) {
        printf("Usage: %s ImageY.pgm ImageCb.pgm ImageCr.pgm ImageOut.ppm\n", argv[0]);
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLueY);
    sscanf(argv[2], "%s", cNomImgLueCb);
    sscanf(argv[3], "%s", cNomImgLueCr);
    sscanf(argv[4], "%s", cNomImgEcrite);

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgInY, *ImgInCb, *ImgInCr, *ImgOut;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLueY, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgInY, OCTET, nTaille);
    allocation_tableau(ImgInCb, OCTET, nTaille);
    allocation_tableau(ImgInCr, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille3);
    lire_image_pgm(cNomImgLueY, ImgInY, nH * nW);
    lire_image_pgm(cNomImgLueCb, ImgInCb, nH * nW);
    lire_image_pgm(cNomImgLueCr, ImgInCr, nH * nW);

    // Parcours de chaque pixel de l'image
    for (int i = 0; i < nTaille3; i += 3) {
        int nR = ImgInY[i / 3] + 1.402 * (ImgInCr[i / 3] - 128);
        int nG = ImgInY[i / 3] - 0.34414 * (ImgInCb[i / 3] - 128) - 0.714414 * (ImgInCr[i / 3] - 128);
        int nB = ImgInY[i / 3] + 1.772 * (ImgInCb[i / 3] - 128);

        if (nR < 0) { nR = 0; }
        if (nR > 255) { nR = 255; }
        if (nG < 0) { nG = 0; }
        if (nG > 255) { nG = 255; }
        if (nB < 0) { nB = 0; }
        if (nB > 255) { nB = 255; }

        ImgOut[i] = nR; // Rouge
        ImgOut[i + 1] = nG; // Vert
        ImgOut[i + 2] = nB; // Bleu
    }

    // Écriture de l'image de sortie et return
    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgInY);
    free(ImgInCb);
    free(ImgInCr);
    free(ImgOut);
    return 0;
}
