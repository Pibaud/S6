// test_couleur.cpp : Seuille une image couleur

#include "image_ppm.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;

  if (argc != 3) {
    printf("Usage: ImageIn.ppm ImageOut.ppm\n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%s", cNomImgEcrite);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  int nTaille3 = nTaille * 3;
  allocation_tableau(ImgIn, OCTET, nTaille3);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille3);

  for (int i = 0; i < nTaille3; i += 3) {
    int rSum = 0, gSum = 0, bSum = 0;
    int count = 0;

    rSum += ImgIn[i];     
    gSum += ImgIn[i + 1]; 
    bSum += ImgIn[i + 2]; 
    count++;

    int voisins[] = {
        -3 * (nW + 1),
        -3 * (nW - 1), 
        3 * (nW - 1),  
        3 * (nW + 1)    
    };

    for (int k = 0; k < 4; k++) {
        int voisinIndex = i + voisins[k];

        if (voisinIndex >= 0 && voisinIndex < nTaille3) {
            rSum += ImgIn[voisinIndex];
            gSum += ImgIn[voisinIndex + 1];
            bSum += ImgIn[voisinIndex + 2];
            count++;
        }
    }

    ImgOut[i] = rSum / count;      
    ImgOut[i + 1] = gSum / count;  
    ImgOut[i + 2] = bSum / count;   
}

  ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  return 1;
}
