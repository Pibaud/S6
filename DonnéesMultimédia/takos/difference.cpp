#include <stdio.h>
#include "image_ppm.h"

/*
Difference entre deux images binaire, pour chaque pixel des deux images, si leur valeur est differente alors on met le pixel en
noir, sinon il est blanc
*/

int main(int argc, char* argv[]) {
    char cNomImgLue1[250], cNomImgLue2[250], cNomImgEcrite[250];
    int nH, nW, nTaille;

    // Vérification des arg
    if (argc != 4) {
        printf("Usage: ImageIn1.pgm ImageIn2.pgm ImageOut.pgm \n");
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue1);
    sscanf(argv[2], "%s", cNomImgLue2);
    sscanf(argv[3], "%s", cNomImgEcrite);

    // Lecture et attributions des dimensions de l'image in1/in2/out
    OCTET *ImgIn1, *ImgIn2, *ImgOut;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn1, OCTET, nTaille);
    allocation_tableau(ImgIn2, OCTET, nTaille);
    lire_image_pgm(cNomImgLue1, ImgIn1, nH * nW);
    lire_image_pgm(cNomImgLue2, ImgIn2, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // Parcours de chaque pixel de l'image
    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            int index = i * nW + j;
            if (ImgIn1[index] == ImgIn2[index]) { // Si couleur de pixel identique alors on le met blanc
                ImgOut[index] = 255;
            } else { // Sinon on le met en noir
                ImgOut[index] = 0;
            }
        }
    }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn1);
    free(ImgIn2);
    free(ImgOut);

    return 0;
}
