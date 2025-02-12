// test_couleur.cpp : Seuille une image en niveau de gris

#include "image_ppm.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char cNomImgLue[250];
  int nH, nW, nTaille, indice;
  bool loc;    // true = ligne, false = colonne
  int tempLoc; // Variable temporaire pour lire la valeur de loc

  if (argc != 4) {
    printf("Usage: %s ImageIn.pgm colOrLine(0:col, 1:line) indice\n", argv[0]);
    exit(1);
  }

  // Lecture des arguments
  sscanf(argv[1], "%s", cNomImgLue);
  sscanf(argv[2], "%d", &tempLoc); // Lecture en tant qu'entier
  loc = (tempLoc != 0);            // Conversion en booléen
  sscanf(argv[3], "%d", &indice);

  // Validation de la valeur de loc
  if (tempLoc != 0 && tempLoc != 1) {
    printf("Erreur: colOrLine doit être 0 (colonne) ou 1 (ligne).\n");
    exit(1);
  }

  printf("Image: %s\n", cNomImgLue);
  printf("Mode: %s\n", loc ? "Ligne" : "Colonne");
  printf("Indice: %d\n", indice);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);

  int t[256] = {0};

  if (loc) { // C'est une ligne
    for (int j = 0; j < nW; j++) {
      t[ImgIn[indice * nW + j]] += 1;
    }
  } else {
    for (int i = 0; i < nH; i++) {
      t[ImgIn[i * nW + indice]] += 1;
    }
  }

  FILE *file = fopen("fich.dat", "w");
  if (file == nullptr) {
    perror("Erreur lors de l'ouverture du fichier");
    return 1;
  }

  for (int i = 0; i < 256; i++) {
    fprintf(file, "%d %d\n", i, t[i]); // Écrit chaque valeur avec son index
  }

  fclose(file);
  free(ImgIn);

  return 1;
}