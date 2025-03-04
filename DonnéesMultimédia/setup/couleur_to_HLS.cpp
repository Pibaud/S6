#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image_ppm.h"

/*
Conversion d'une image PPM en trois images PGM correspondant aux composantes H (teinte),
L (luminance) et S (saturation) de l'espace de couleur HLS.
Formules utilisées :
- r = R/255, g = G/255, b = B/255
- max = max(r, g, b) et min = min(r, g, b)
- L = (max + min) / 2
- Si max == min alors H = 0 et S = 0
  Sinon :
    delta = max - min
    S = (delta / (max + min)) si L < 0.5, sinon S = (delta / (2 - max - min))
    Si max == r alors H = 60 * ( (g - b)/delta mod 6 )
    Si max == g alors H = 60 * ( (b - r)/delta + 2 )
    Si max == b alors H = 60 * ( (r - g)/delta + 4 )
- Mise à l'échelle : H en [0,360] -> H_out = (H/360)*255, L_out = L*255, S_out = S*255
*/

int main(int argc, char *argv[]) {
    char cNomImgLue[250], cNomImgEcriteH[250], cNomImgEcriteL[250], cNomImgEcriteS[250];
    int nH, nW, nTaille, nR, nG, nB;

    // Vérification du nombre d'arguments
    if (argc != 5) {
        printf("Usage: %s ImageIn.ppm ImageH.pgm ImageL.pgm ImageS.pgm\n", argv[0]);
        exit(1);
    }

    // Affectation des noms de fichiers
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcriteH);
    sscanf(argv[3], "%s", cNomImgEcriteL);
    sscanf(argv[4], "%s", cNomImgEcriteS);

    // Allocation et lecture de l'image
    OCTET *ImgIn, *ImgOutH, *ImgOutL, *ImgOutS;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    allocation_tableau(ImgOutH, OCTET, nTaille);
    allocation_tableau(ImgOutL, OCTET, nTaille);
    allocation_tableau(ImgOutS, OCTET, nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nTaille);

    // Conversion de chaque pixel de RGB vers HLS
    for (int i = 0; i < nTaille3; i += 3) {
        nR = ImgIn[i];     // composante Rouge
        nG = ImgIn[i + 1]; // composante Vert
        nB = ImgIn[i + 2]; // composante Bleu

        // Conversion en valeurs normalisées
        double r = nR / 255.0;
        double g = nG / 255.0;
        double b = nB / 255.0;

        // Calcul de max et min
        double max = fmax(r, fmax(g, b));
        double min = fmin(r, fmin(g, b));
        double L = (max + min) / 2.0;  // luminance

        double H, S;
        if (max == min) { // cas de teinte indéfinie
            H = 0;
            S = 0;
        } else {
            double delta = max - min;
            // Calcul de la saturation
            if (L < 0.5)
                S = delta / (max + min);
            else
                S = delta / (2.0 - max - min);
            
            // Calcul de la teinte
            if (max == r) {
                H = 60 * fmod(((g - b) / delta), 6);
                if (H < 0)
                    H += 360;
            } else if (max == g) {
                H = 60 * (((b - r) / delta) + 2);
            } else { // max == b
                H = 60 * (((r - g) / delta) + 4);
            }
        }
        
        // Mise à l'échelle de H, L et S vers l'intervalle [0, 255]
        int H_out = (int)(H / 360.0 * 255.0);
        int L_out = (int)(L * 255.0);
        int S_out = (int)(S * 255.0);
        
        // Assurer que les valeurs sont bien comprises entre 0 et 255
        if (H_out < 0)   H_out = 0;
        if (H_out > 255) H_out = 255;
        if (L_out < 0)   L_out = 0;
        if (L_out > 255) L_out = 255;
        if (S_out < 0)   S_out = 0;
        if (S_out > 255) S_out = 255;

        ImgOutH[i / 3] = H_out;
        ImgOutL[i / 3] = L_out;
        ImgOutS[i / 3] = S_out;
    }

    // Écriture des images de sortie
    ecrire_image_pgm(cNomImgEcriteH, ImgOutH, nH, nW);
    ecrire_image_pgm(cNomImgEcriteL, ImgOutL, nH, nW);
    ecrire_image_pgm(cNomImgEcriteS, ImgOutS, nH, nW);

    free(ImgIn);
    free(ImgOutH);
    free(ImgOutL);
    free(ImgOutS);
    return 0;
}