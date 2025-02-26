// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250], nomHistogramme[250];
    int nH, nW, nTaille;

    if (argc != 3)
    {
        printf("Usage: ImageIn.pgm Histogramme.dat\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", nomHistogramme);

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);

    //   for (int i=0; i < nTaille; i++)
    // {
    //  if ( ImgIn[i] < S) ImgOut[i]=0; else ImgOut[i]=255;
    //  }

    int t[256] = {0};

    for (int i = 0; i < nH; i++)
        for (int j = 0; j < nW; j++)
        {
            t[ImgIn[i * nW + j]] += 1;
        }

    FILE* file = fopen(nomHistogramme, "w");
    if (file == nullptr) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    for (int i = 0; i < 256; i++) {
        fprintf(file, "%d %d\n", i, t[i]); // Écrit chaque valeur avec son index
    }

    fclose(file);
    free(ImgIn);

    return 1;
}