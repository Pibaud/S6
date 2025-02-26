#include "image_ppm.h"
#include <stdio.h>
#include <algorithm>

int main(int argc, char *argv[])
{
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
    if (argc != 3)
    {
        printf("Usage: ImageIn.pgm ImageOut.pgm\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);

    OCTET *ImgIn, *ImgEroded, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgEroded, OCTET, nTaille);
    allocation_tableau(ImgOut, OCTET, nTaille);

    // Erosion
    for (int i = 1; i < nH - 1; i++)
        for (int j = 1; j < nW - 1; j++)
        {
            int minVal = std::min({ImgIn[(i - 1) * nW + (j - 1)], ImgIn[(i - 1) * nW + j], ImgIn[(i - 1) * nW + (j + 1)],
                                   ImgIn[i * nW + (j - 1)], ImgIn[i * nW + j], ImgIn[i * nW + (j + 1)],
                                   ImgIn[(i + 1) * nW + (j - 1)], ImgIn[(i + 1) * nW + j], ImgIn[(i + 1) * nW + (j + 1)]});
            ImgEroded[i * nW + j] = minVal;
        }

    // Dilation
    for (int i = 1; i < nH - 1; i++)
        for (int j = 1; j < nW - 1; j++)
        {
            int maxVal = std::max({ImgEroded[(i - 1) * nW + (j - 1)], ImgEroded[(i - 1) * nW + j], ImgEroded[(i - 1) * nW + (j + 1)],
                                   ImgEroded[i * nW + (j - 1)], ImgEroded[i * nW + j], ImgEroded[i * nW + (j + 1)],
                                   ImgEroded[(i + 1) * nW + (j - 1)], ImgEroded[(i + 1) * nW + j], ImgEroded[(i + 1) * nW + (j + 1)]});
            ImgOut[i * nW + j] = maxVal;
        }

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
    free(ImgEroded);
    free(ImgOut);
    return 1;
}
