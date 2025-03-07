// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char *argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  float ddp[256] = {0};
  float repart[256] = {0};
  int hist[256] = {0};


  if (argc != 3)
  {
    printf("Usage: ImageIn.pgm ImageOut.pgm\n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);

    for (int i = 0; i < nH; i++)
    {
        for (int j = 0; j < nW; j++)
        {
            hist[ImgIn[i * nW + j]]++; 
        }
    }

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

  for (int i = 0; i < nH; i++)
    for (int j = 0; j < nW; j++)
    {
        ImgOut[i * nW + j] = (int)(repart[ImgIn[i * nW + j]] * 255);
    }

  ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(ImgOut);

  return 1;
}
