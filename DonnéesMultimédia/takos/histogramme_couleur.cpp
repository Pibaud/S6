#include <stdio.h>
#include "image_ppm.h"

/*
Histogramme : Notation dans un fichier dat de chaque occurences de valeur de rouge, vert et bleu de l'image ppm
*/

int main(int argc, char* argv[]) {
    char cNomImgLue[250]; char filename[250];
    int nH, nW, nTaille;
    int histoR[256] = {0};
    int histoG[256] = {0};
    int histoB[256] = {0};

    // Vérification des arg
    if (argc != 3) {
        printf("Usage: ImageIn.ppm filename.dat \n");
        exit(1);
    }

    // Affectation des arguments
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", filename);

    // Lecture et attributions des dimensions de l'image in
    OCTET *ImgIn;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    // Définition des occurences de couleurs rgb de chaque pixel dans histoR/G/B[]
    // histoR/G/B[0...255] = nbr occurences
    for (int i = 0; i < nTaille3; i+=3) {
        histoR[ImgIn[i]]++;
        histoG[ImgIn[i+1]]++;
        histoB[ImgIn[i+2]]++;
    }

    // Creation du fichier out
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(1);
    }

    // Ecriture dans le fichier pour chaque pixel, son nombre d'occurences RGB a partir de histoR/G/B
    for (int i = 0; i < 256; i++) {
        fprintf(file, "%d %d %d %d\n", i, histoR[i], histoG[i], histoB[i]);
        // printf("Indice : %d - R : %d | G : %d | B : %d\n", i, histoR[i], histoG[i], histoB[i]);
    }

    // Return
    free(ImgIn);
    fclose(file);
    return 0;
}
