#include <stdio.h>
#include "image_ppm.h"
#include "erosion_dilatation.h"

/*
Ouverture d'une image pgm, application d'une erosion puis d'une dilatation
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

    // Lecture et attributions des dimensions de l'image in/between/out
    OCTET *ImgIn, *ImgEr, *ImgOut;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgEr, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // Application de la fonction erosion et dilatation
    erosion(ImgIn, ImgEr, nH, nW);
    dilatation(ImgEr, ImgOut, nH, nW);

    // Écriture de l'image de sortie et return
    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgEr);
    free(ImgOut);
    return 0;
}
