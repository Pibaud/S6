// test_couleur.cpp : Seuille une image en niveau de gris

#include "image_ppm.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char cNomImgEcrite[250];
  int nH, nW;

  if (argc != 1) {
    printf("Usage:\n");
    exit(1);
  }

  sscanf(argv[4], "%s", cNomImgEcrite);

  OCTET *Y, *Cb, *Cr, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm((char *)"Y.pgm", &nH, &nW);
  int nTaille = nH * nW;
  int nTaille3 = nTaille * 3;

  allocation_tableau(Y, OCTET, nTaille);
  allocation_tableau(Cb, OCTET, nTaille);
  allocation_tableau(Cr, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille3);

  lire_image_pgm((char *)"Y.pgm", Y, nH * nW);
  lire_image_pgm((char *)"Cb.pgm", Cb, nH * nW);
  lire_image_pgm((char *)"Cr.pgm", Cr, nH * nW);

  for (int i = 0; i < nTaille3; i += 3) {
    int r = Y[i/3] + 1.402*(Cr[i/3] - 128);
    if(r<0)r = 0; 
    if(r>255) r=255;
    int g = Y[i/3] - 0.34414*(Cb[i/3] - 128) - 0.714414*(Cr[i/3] - 128);
    if(g<0)g = 0; 
    if(g>255) g=255;
    int b = Y[i/3] + 1.772*(Cb[i/3] - 128);
    if(b<0)b = 0; 
    if(b>255) b=255;
    ImgOut[i] = r;
    ImgOut[i+1] = g;
    ImgOut[i+2] = b;
  }

  ecrire_image_ppm((char *)"ppmOut.ppm", ImgOut, nH, nW);
  free(Y);
  free(Cb);
  free(Cr);
  free(ImgOut);

  return 1;
}