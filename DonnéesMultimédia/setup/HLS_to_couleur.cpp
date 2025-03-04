#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image_ppm.h"

/*
Reconstruction d'une image couleur PPM à partir de 3 images PGM correspondant aux
composantes H, L et S.
Les images H, L, S ont été obtenues par une conversion de RGB -> HLS.
La conversion inverse (HLS -> RGB) s'effectue de la manière suivante :
1. Conversion en valeurs normalisées :
   h = H/255, l = L/255, s = S/255   (où h correspond à H/360)
2. Si s == 0, alors r = g = b = l (image en niveaux de gris)
3. Sinon :
   - Si l < 0.5, q = l * (1 + s), sinon q = l + s - l * s.
   - p = 2 * l - q.
   - r = hue2rgb(p, q, h + 1/3)
   - g = hue2rgb(p, q, h)
   - b = hue2rgb(p, q, h - 1/3)
4. Mise à l'échelle de r, g, b de [0, 1] à [0, 255].
*/

/* Fonction auxiliaire pour la conversion HLS -> RGB */
double hue2rgb(double p, double q, double t) {
    if(t < 0)
        t += 1;
    if(t > 1)
        t -= 1;
    if(t < 1.0/6.0)
        return p + (q - p) * 6 * t;
    if(t < 1.0/2.0)
        return q;
    if(t < 2.0/3.0)
        return p + (q - p) * (2.0/3.0 - t) * 6;
    return p;
}

int main(int argc, char *argv[]) {
    char cNomImgH[250], cNomImgL[250], cNomImgS[250], cNomImgOut[250];
    int nH, nW, nTaille;

    // Vérification du nombre d'arguments
    if (argc != 5) {
        printf("Usage: %s ImageH.pgm ImageL.pgm ImageS.pgm ImageOut.ppm\n", argv[0]);
        exit(1);
    }

    // Affectation des noms de fichiers
    sscanf(argv[1], "%s", cNomImgH);
    sscanf(argv[2], "%s", cNomImgL);
    sscanf(argv[3], "%s", cNomImgS);
    sscanf(argv[4], "%s", cNomImgOut);

    // Lecture des dimensions de l'image (on suppose que les 3 images ont les mêmes dimensions)
    lire_nb_lignes_colonnes_image_pgm(cNomImgH, &nH, &nW);
    nTaille = nH * nW;

    // Allocation des tableaux pour les images H, L et S
    OCTET *ImgH, *ImgL, *ImgS;
    allocation_tableau(ImgH, OCTET, nTaille);
    allocation_tableau(ImgL, OCTET, nTaille);
    allocation_tableau(ImgS, OCTET, nTaille);

    // Lecture des images H, L, S
    lire_image_pgm(cNomImgH, ImgH, nTaille);
    lire_image_pgm(cNomImgL, ImgL, nTaille);
    lire_image_pgm(cNomImgS, ImgS, nTaille);

    // Allocation pour l'image de sortie en couleur (PPM)
    int nTaille3 = nTaille * 3;
    OCTET *ImgOut;
    allocation_tableau(ImgOut, OCTET, nTaille3);

    // Reconstruction de chaque pixel en RGB
    for (int i = 0; i < nTaille; i++) {
        // Récupération des valeurs H, L et S depuis les images (valeurs entières 0-255)
        int H_val = ImgH[i];
        int L_val = ImgL[i];
        int S_val = ImgS[i];

        // Conversion en valeurs normalisées (0 à 1)
        double h = H_val / 255.0; // correspond à H/360 en fraction
        double l = L_val / 255.0;
        double s = S_val / 255.0;

        double r, g, b;

        if (s == 0) {
            // Image en niveaux de gris
            r = g = b = l;
        } else {
            double q = (l < 0.5) ? (l * (1 + s)) : (l + s - l * s);
            double p = 2 * l - q;
            r = hue2rgb(p, q, h + 1.0/3.0);
            g = hue2rgb(p, q, h);
            b = hue2rgb(p, q, h - 1.0/3.0);
        }

        // Mise à l'échelle de [0,1] à [0,255]
        int R = (int)(r * 255.0);
        int G = (int)(g * 255.0);
        int B = (int)(b * 255.0);

        // S'assurer que les valeurs sont dans l'intervalle [0, 255]
        if(R < 0) R = 0; if(R > 255) R = 255;
        if(G < 0) G = 0; if(G > 255) G = 255;
        if(B < 0) B = 0; if(B > 255) B = 255;

        // Stockage dans l'image de sortie (format PPM : R, G, B)
        ImgOut[3*i]     = R;
        ImgOut[3*i + 1] = G;
        ImgOut[3*i + 2] = B;
    }

    // Écriture de l'image PPM reconstruite
    ecrire_image_ppm(cNomImgOut, ImgOut, nH, nW);

    // Libération de la mémoire
    free(ImgH);
    free(ImgL);
    free(ImgS);
    free(ImgOut);

    return 0;
}
