#include "image_ppm.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char cNomImgLue[250];
  int nH, nW, nTaille;

  if (argc != 2) {
    printf("Usage: ImageIn.pgm\n");
    exit(1);
  }

  sscanf(argv[1], "%s", cNomImgLue);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  int nTaille3 = nTaille * 3;
  allocation_tableau(ImgIn, OCTET, nTaille3);
  lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille3);

  int r[256] = {0};
  int g[256] = {0};
  int b[256] = {0};

  for (int i = 0; i < nTaille3; i += 3) {
    r[ImgIn[i]] += 1;
    g[ImgIn[i + 1]] += 1;
    b[ImgIn[i + 2]] += 1;
  }

  FILE *file = fopen("histo3.dat", "w");
  if (file == nullptr) {
    perror("Erreur lors de l'ouverture du fichier");
    return 1;
  }

  for (int i = 0; i < 256; i++) {
    fprintf(file, "%d %d %d %d\n", i, r[i], g[i],
            b[i]); // Écrit chaque valeur avec son index
  }

  fclose(file);
  free(ImgIn);

  return 1;
}