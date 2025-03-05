#include <stdio.h>
#include "image_ppm.h"

/*
Conversion d'une image ppm en 3 images pgm Y, Cb, Cr, pour chaque pixel, on applique des formules mathematiques
en modifiant le pourcentage de rouge, vert, bleu que l'on va mettre dans chaque image 
*/

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgEcriteY[250], cNomImgEcriteCb[250], cNomImgEcriteCr[250];
    int nH, nW, nTaille, nR, nG, nB;

    // Vérification des arg
    if (argc != 5) {
        printf("Usage: %s ImageIn.ppm ImageY.pgm ImageCb.pgm ImageCr.pgm\n", argv[0]);
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcriteY);
    sscanf(argv[3], "%s", cNomImgEcriteCb);
    sscanf(argv[4], "%s", cNomImgEcriteCr);

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgIn, *ImgOutY, *ImgOutCb, *ImgOutCr;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    allocation_tableau(ImgOutY, OCTET, nTaille);
    allocation_tableau(ImgOutCb, OCTET, nTaille);
    allocation_tableau(ImgOutCr, OCTET, nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    // Parcours de chaque pixel de l'image
    for (int i = 0; i < nTaille3; i += 3) {
        nR = ImgIn[i]; // Rouge
        nG = ImgIn[i + 1]; // Vert
        nB = ImgIn[i + 2]; // Bleu

        int Y = (0.299 * nR) + (0.587 * nG) + (0.114 * nB);
        int Cb = (-0.1687 * nR) - (0.3313 * nG) + (0.5 * nB) + 128;
        int Cr = (0.5 * nR) - (0.4187 * nG) - (0.0813 * nB) + 128;

        if (Y < 0) { Y = 0; }
        if (Y > 255) { Y = 255; }
        if (Cb < 0) { Cb = 0; }
        if (Cb > 255) { Cb = 255; }
        if (Cr < 0) { Cr = 0; }
        if (Cr > 255) { Cr = 255; }

        ImgOutY[i / 3] = Y;
        ImgOutCb[i / 3] = Cb;
        ImgOutCr[i / 3] = Cr;
    }

    // Écriture de l'image de sortie et return
    ecrire_image_pgm(cNomImgEcriteY, ImgOutY, nH, nW);
    ecrire_image_pgm(cNomImgEcriteCb, ImgOutCb, nH, nW);
    ecrire_image_pgm(cNomImgEcriteCr, ImgOutCr, nH, nW);
    free(ImgIn);
    free(ImgOutY);
    free(ImgOutCb);
    free(ImgOutCr);
    return 0;
}
