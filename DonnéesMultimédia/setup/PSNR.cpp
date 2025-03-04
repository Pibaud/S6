//fait les calculs des formules du cours

#include <cstdio>
#include <stdio.h>
#include "image_ppm.h"
#include <iostream>
using namespace std;

double distancequadratique(OCTET *ImgIn1, OCTET *ImgIn2, int nTaille) {
    double distquad = 0.0;
    for (int i = 0; i < nTaille; i++) {
        double diff = ImgIn1[i] - ImgIn2[i];
        distquad += diff * diff;
    }
    return distquad/nTaille;
}

int psnr(OCTET *ImgIn1, OCTET *ImgIn2, int nTaille) {
    double distquad = distancequadratique(ImgIn1, ImgIn2, nTaille);
    return 10.0 * log10((255 * 255) / (distquad));
}



int main(int argc, char* argv[])
{
    char cNomImgLue1[250], cNomImgLue2[250];
    int nH1, nW1, nH2, nW2;

    if (argc != 3 )  
    {
        printf("Usage: ImageIn1.pgm ImageIn2.pgm \n"); 
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue1);
    sscanf(argv[2], "%s", cNomImgLue2);

    double distquad = 0.0;
    OCTET *ImgIn1, *ImgIn2;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH1, &nW1);
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue2, &nH2, &nW2);

    if (nH1 != nH2 || nW1 != nW2) {
        cerr << "Les images n'ont pas la même taille" << endl;
        return 1;
    }

    int nTaille = nH1 * nW1;

    allocation_tableau(ImgIn1, OCTET, nTaille);
    allocation_tableau(ImgIn2, OCTET, nTaille);

    if (ImgIn1 == nullptr || ImgIn2 == nullptr) {
        cerr << "Erreur d'allocation mémoire" << endl;
        return 1;
    }

    lire_image_pgm(cNomImgLue1, ImgIn1, nTaille);
    lire_image_pgm(cNomImgLue2, ImgIn2, nTaille);

    distquad = distancequadratique(ImgIn1, ImgIn2, nTaille);

    cout << "Distance quadratique : " << distquad << endl;

    int psnrValue = psnr(ImgIn1, ImgIn2, nTaille);

    cout << "PSNR : " << psnrValue << " dB" << endl;

    free(ImgIn1);
    free(ImgIn2);

    return 0;
}