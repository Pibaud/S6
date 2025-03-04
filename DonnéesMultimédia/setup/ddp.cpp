// test_couleur.cpp : Seuille une image en niveau de gris

#include <fstream>
#include <iostream>
#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250], nomHistogramme[250];
    int nH, nW, nTaille;

    if (argc != 3)
    {
        printf("Usage: ImageIn.pgm nomHistoSortie.dat\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", nomHistogramme);

    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    int t[256] = {0};

    for (int i = 0; i < nH; i++)
        for (int j = 0; j < nW; j++)
        {
            t[ImgIn[i * nW + j]] += 1;
        }

    std::ofstream file(nomHistogramme);
    for(int i = 0; i < 256; i++){
        file << i << "\t" << (float)t[i]/(float)nTaille << "\n";
    }

    std::cout << "fini !" << std::endl;

    file.close();    
    free(ImgIn);

    return 1;
}