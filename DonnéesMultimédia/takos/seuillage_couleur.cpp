#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, nR, nG, nB, S;

    // Vérification des arg
    if (argc != 4) {
        printf("Usage: %s ImageIn.ppm ImageOut.ppm Seuil\n", argv[0]);
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &S);

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgIn, *ImgOut;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    allocation_tableau(ImgOut, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    // Parcours de chaque pixel de l'image
    for (int i = 0; i < nTaille3; i += 3) {
        nR = ImgIn[i];
        nG = ImgIn[i + 1];
        nB = ImgIn[i + 2];
        ImgOut[i] = (nR < S) ? 0 : 255; // Rouge
        ImgOut[i + 1] = (nG < S) ? 0 : 255; // Vert
        ImgOut[i + 2] = (nB < S) ? 0 : 255; // Bleu
    }

    // Écriture de l'image de sortie et return
    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
