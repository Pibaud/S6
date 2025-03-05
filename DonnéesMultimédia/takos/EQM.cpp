#include <stdio.h>
#include "image_ppm.h"

/*
Calcul de l'EQM et du PSNR entre deux images pgm, une image ppm convertie en pgm a l'aide de GIMP par exemple et une image 
convertie à l'aide du programme couleur_to_gris.cpp, et caclul pour chaque pixel la difference de valeur entre les deux. 
Pour l'EQM plus la valeur est proche de 0, plus la ressemblance entre les deux images est forte.
Pour le PSNR (valeur exprimé en dB), plus la valeur est haute mieux c'est. Si > 35dB alors aucunes différences visibles
. Si < 20dB alors grosse différences visible
*/

int main(int argc, char *argv[]) {
    char cNomImgLue1[250], cNomImgLue2[250];
    int nH, nW, nTaille, nR, nG, nB;

    // Vérification des arg
    if (argc != 3) {
        printf("Usage: %s ImageIn1_Original.pgm ImageIn2_ConvertWithCPP.pgm\n", argv[0]);
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue1);
    sscanf(argv[2], "%s", cNomImgLue2);

    // Lecture et attributions des dimensions de l'image in/out
    OCTET *ImgIn1, *ImgIn2;
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);
    nTaille = nH * nW;
    allocation_tableau(ImgIn1, OCTET, nTaille);
    allocation_tableau(ImgIn2, OCTET, nTaille);
    lire_image_pgm(cNomImgLue1, ImgIn1, nH * nW);
    lire_image_pgm(cNomImgLue2, ImgIn2, nH * nW);

    // Parcours de chaque pixel de l'image
    double sum = 0.0;
    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            int pixel_Img1 = ImgIn1[i * nW + j]; // Valeur de pixel de l'image 1
            int pixel_Img2 = ImgIn2[i * nW + j]; // Valeur de pixel de l'image 2

            double diff = pixel_Img1 - pixel_Img2;
            sum += diff * diff;
        }
    }

    // Calcul de l'EQM et du PSNR
    double EQM = (1 / nTaille) * sum; // (1/p) * Σ(a - b)**2 | p = nombre total de pixels. a/b = même pixel des deux images
    double PSNR = 10 * log10((255.0 * 255.0 )/ EQM); // 10 * log10((d**2) / EQM) | d = valeur maximal d'un pixel

    // Affichage de l'EQM et du PSNR
    printf("EQM : %lf\n", EQM);
    printf("PSNR : %lf dB\n", PSNR);

    // Return
    free(ImgIn1);
    free(ImgIn2);
    return 0;
}
