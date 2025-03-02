// To compile and execute this code on Windows:
// 1. Open Command Prompt.
// 2. Navigate to the directory containing this file.
// 3. Compile the code using: g++ -o dilatationPPM dilatationPPM.cpp image_ppm.cpp
// 4. Run the executable using: dilatationPPM ImageIn.ppm ImageOut.ppm

#include "image_ppm.h"
#include <stdio.h>
#include <algorithm>

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
  nTaille = nH * nW * 3;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);

  for (int i = 1; i < nH-1; i++) {
    for (int j = 1; j < nW-1; j++) {
      for (int k = 0; k < 3; k++) {
        int maxVal = std::max({
            ImgIn[((i-1)*nW+(j-1))*3+k], ImgIn[((i-1)*nW+j)*3+k], ImgIn[((i-1)*nW+(j+1))*3+k],
            ImgIn[(i*nW+(j-1))*3+k], ImgIn[(i*nW+j)*3+k], ImgIn[(i*nW+(j+1))*3+k],
            ImgIn[((i+1)*nW+(j-1))*3+k], ImgIn[((i+1)*nW+j)*3+k], ImgIn[((i+1)*nW+(j+1))*3+k]
        });
        ImgOut[(i*nW+j)*3+k] = maxVal;
      }
    }
  }

  ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
  free(ImgIn);
  free(ImgOut);

  return 1;
}