#include "image_ppm.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char cNomImgSeuillee[250], cNomImgDilatee[250], cNomImgEcrite[250];
  int nH, nW, nTaille;

  if (argc != 4) {
    printf("Usage: Image_seuillee Image_dilatee ImageOut.pgm\n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgSeuillee);
  sscanf(argv[2], "%s", cNomImgDilatee);
  sscanf(argv[3], "%s", cNomImgEcrite);

  OCTET *ImgS, *ImgD, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgSeuillee, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgS, OCTET, nTaille);
  lire_image_pgm(cNomImgSeuillee, ImgS, nH * nW);
  allocation_tableau(ImgD, OCTET, nTaille);
  lire_image_pgm(cNomImgDilatee, ImgD, nH * nW);

  allocation_tableau(ImgOut, OCTET, nTaille);

  for (int i = 1; i < nH - 1; i++)
    for (int j = 1; j < nW - 1; j++) {
      if (ImgS[i * nW + j] == ImgD[i * nW + j]) {
        ImgOut[i * nW + j] = 255;
      }
      else{
        ImgOut[i * nW + j] = 0;
      }
    }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgS);
  free(ImgD);
  free(ImgOut);

  return 1;
}