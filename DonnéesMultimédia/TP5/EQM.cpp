#include "image_ppm.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char cNomImg1[250], cNomImg2[250];
  int nH1, nW1, nTaille1;

  if (argc != 3) {
    printf("Usage: Image1.pgm Image2.pgm\n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImg1);
  sscanf(argv[2], "%s", cNomImg2);

  OCTET *Img1, *Img2;

  lire_nb_lignes_colonnes_image_pgm(cNomImg1, &nH1, &nW1);
  nTaille1 = nH1 * nW1;

  allocation_tableau(Img1, OCTET, nTaille1);
  lire_image_pgm(cNomImg1, Img1, nH1 * nW1);
  allocation_tableau(Img2, OCTET, nTaille1);
  lire_image_pgm(cNomImg2, Img2, nH1 * nW1);

  double sommeDiff = 0.0;

  for (int i = 0; i < nH1; i++)
    for (int j = 0; j < nW1; j++) {
        sommeDiff += (Img1[i * nW1 + j] - Img2[i * nW1 + j]) * (Img1[i * nW1 + j] - Img2[i * nW1 + j]);
    }

  float moyenne = (float)sommeDiff/nTaille1;
  printf("EQM = %f\n", moyenne);

  free(Img1);
  free(Img2);

  return 1;
}