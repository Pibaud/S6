/*
set grid
plot "repartition.dat" using 1:2 with lines
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
    int hist[256] = {0};
    char cNomImgLue[250];
    int nH, nW, nTaille;

    if (argc != 2) {
        printf("Usage: %s ImageIn.pgm\n", argv[0]);
        return 1;
    }

    sscanf(argv[1], "%s", cNomImgLue);

    OCTET *ImgIn;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nTaille);

    for (int i = 0; i < nH; i++)
    {
        for (int j = 0; j < nW; j++)
        {
            hist[ImgIn[i * nW + j]]++; 

        }
    }

    std::ofstream file("repartition.dat");

    if (!file) {
        std::cerr << "Erreur d'ouverture du fichier !" << std::endl;
        return 1;
    }

    float ddp[256] = {0};
    float repart[256] = {0};

    for (int i = 0; i < 256; i++) {
        ddp[i] = ((float)hist[i]/(float)nTaille);
    }

    for (int i = 0; i < 256; i++) {
        if(i == 0){
            repart[i] = ddp[i];
        }
        else{
            repart[i] = repart[i-1] + ddp[i];
        }
    }

    for (int i = 0; i < 256; i++) {
        file << i << "\t" << repart[i] << "\n";
    }

    file.close();
    std::cout << "Les données ont été écrites dans 'repartition.dat'." << std::endl;

    free(ImgIn);

    return 0;
}
