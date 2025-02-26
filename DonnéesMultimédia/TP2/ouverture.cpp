#include "image_ppm.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;

  if (argc != 3) {
    printf("Usage: ImageIn.pgm ImageOut.pgm\n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut, *ImgTemp;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);
  allocation_tableau(ImgTemp, OCTET, nTaille);

  // Dilation
  for (int i = 1; i < nH-1; i++) {
    for (int j = 1; j < nW-1; j++) {
      if (ImgIn[(i-1)*nW+(j-1)] == 255 ||
          ImgIn[(i-1)*nW+j] == 255 ||
          ImgIn[(i-1)*nW+(j+1)] == 255 ||
          ImgIn[i*nW+(j-1)] == 255 ||
          ImgIn[i*nW+(j+1)] == 255 ||
          ImgIn[(i+1)*nW+(j-1)] == 255 ||
          ImgIn[(i+1)*nW+j] == 255 ||
          ImgIn[(i+1)*nW+(j+1)] == 255) {
        ImgTemp[i*nW+j] = 255;
      }
    }
  }

  // Erosion
  for (int i = 1; i < nH-1; i++) {
    for (int j = 1; j < nW-1; j++) {
      if (ImgTemp[(i-1)*nW+(j-1)] == 0 ||
          ImgTemp[(i-1)*nW+j] == 0 ||
          ImgTemp[(i-1)*nW+(j+1)] == 0 ||
          ImgTemp[i*nW+(j-1)] == 0 ||
          ImgTemp[i*nW+(j+1)] == 0 ||
          ImgTemp[(i+1)*nW+(j-1)] == 0 ||
          ImgTemp[(i+1)*nW+j] == 0 ||
          ImgTemp[(i+1)*nW+(j+1)] == 0) {
        ImgOut[i*nW+j] = 0;
      }
    }
  }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(ImgOut);
  free(ImgTemp);

  return 1;
}