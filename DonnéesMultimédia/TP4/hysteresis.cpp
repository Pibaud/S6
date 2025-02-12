// test_couleur.cpp : Seuille une image en niveau de gris

#include "image_ppm.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S1, S2;

  if (argc != 5) {
    printf("Usage: ImageIn.pgm ImageOut.pgm Seuil1 Seuil2\n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);
  sscanf(argv[3], "%d", &S1);
  sscanf(argv[4], "%d", &S2);

  OCTET *ImgIn, *ImgTemp, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);
  allocation_tableau(ImgTemp, OCTET, nTaille);

  //   for (int i=0; i < nTaille; i++)
  // {
  //  if ( ImgIn[i] < S) ImgOut[i]=0; else ImgOut[i]=255;
  //  }

  for (int i = 0; i < nH; i++) {
    for (int j = 0; j < nW; j++) {
      if (ImgIn[i * nW + j] < S1)
        ImgTemp[i * nW + j] = 0;
      if (ImgIn[i * nW + j] > S2)
        ImgTemp[i * nW + j] = 255;
    }
  }

  for (int i = 0; i < nH; i++) {
    for (int j = 0; j < nW; j++) {
      if ((S1 < ImgTemp[i * nW + j] &&
          ImgTemp[(i-1) * nW + j-1] < S2) &&
          (ImgTemp[(i-1) * nW + j] == 255 ||
          ImgTemp[(i-1) * nW + j+1] == 255 ||
          ImgTemp[i * nW + j-1] == 255 ||
          ImgTemp[i * nW + j+1] == 255 ||
          ImgTemp[(i+1) * nW + j-1] == 255 ||
          ImgTemp[(i+1) * nW + j] == 255 ||
          ImgTemp[(i+1) * nW + j+1] == 255)) {
        ImgOut[i * nW + j] = 255;
      } else {
        ImgOut[i * nW + j] = 0;
      }
    }
  }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(ImgOut);

  return 1;
}