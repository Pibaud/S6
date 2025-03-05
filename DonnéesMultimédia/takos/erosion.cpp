#include <stdio.h>
#include "image_ppm.h"

/*
Erosion d'une image binaire, pour chaque pixel, si il est noir, alors si un de ses voisins est blanc, il devient blanc, sinon il
reste noir, si il est blanc alors il reste blanc. Permet de faire grossir le fond dans l'image
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

    // Lecture et attributions des dimensions de l'image in/out
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
            if (ImgIn[index] == 0) { // Si un pixel noir a un voisin blanc, alors il devient blanc
                if (ImgIn[(i-1) * nW + j] == 255
                || ImgIn[(i+1) * nW + j] == 255
                || ImgIn[i * nW + (j-1)] == 255
                || ImgIn[i * nW + (j+1)] == 255) {
                    ImgOut[index] = 255;
                } else { // Sinon il reste noir
                    ImgOut[index] = 0;
                }
            } else { // Sinon il reste pixel blanc
                ImgOut[index] = 255;
            }
        }
    }

    // Écriture de l'image de sortie et return
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
