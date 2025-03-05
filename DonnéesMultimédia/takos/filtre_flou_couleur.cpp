#include <stdio.h>
#include "image_ppm.h"

/*
Application d'un filtre flou a une image ppm, remplacement de chaque pixel par la valeur moyenne des ses 4 voisins dans
son domaine de couleur
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

    // Lecture et attributions des dimensions de l'image in1/in2/out
    OCTET *ImgIn, *ImgOut;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);

    // Parcours de chaque pixel de l'image
    for (int i = 3; i < nTaille3 - 3; i += 3) {
        int indexR = i;
        int indexG = i+1;
        int indexB = i+2;
        ImgOut[i] = (ImgIn[indexR] + ImgIn[indexR - 3] + ImgIn[indexR + 3] + ImgIn[indexR - nW*3] + ImgIn[indexR + nW*3]) / 5; // Rouge
        ImgOut[i + 1] = (ImgIn[indexG] + ImgIn[indexG - 3] + ImgIn[indexG + 3] + ImgIn[indexG - nW*3] + ImgIn[indexG + nW*3]) / 5; // Vert
        ImgOut[i + 2] = (ImgIn[indexB] + ImgIn[indexB - 3] + ImgIn[indexB + 3] + ImgIn[indexB - nW*3] + ImgIn[indexB + nW*3]) / 5; // Bleu
    }

    // Ecriture dans l'image de sortie et return
    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgOut);
    return 0;
}
