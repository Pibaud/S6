#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, k;

    // Vérification des arg
    if (argc != 4) {
        printf("Usage: %s ImageIn.pgm ImageOut.ppm k\n", argv[0]);
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &k);

    // Verification de la valeur k
    if (k < -128 || k > 128) {
        printf("Error : -128 < k < 128\n");
        exit(1);
    }

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgIn, *ImgOut;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    // Parcours de chaque pixel de l'image
    for (int i = 0; i < nH; i ++) {
        for (int j = 0; j < nW; j++) {
            int pixel = i * nH + j;

            int res = ImgIn[pixel] + k;
            if (res < 0) { res = 0; }
            if (res > 255) { res = 255; }

            ImgOut[pixel] = res;
        }
    }

    // Écriture de l'image de sortie et return
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
