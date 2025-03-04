// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250];
  int nH, nW, nTaille;
  
  if (argc != 2) 
     {
       printf("Usage: ImageIn.ppm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue);

   OCTET *ImgIn, *ImgY, *ImgCr, *ImgCb;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgY, OCTET, nTaille);
   allocation_tableau(ImgCr, OCTET, nTaille);
   allocation_tableau(ImgCb, OCTET, nTaille);
	
   for (int i=0; i < nTaille3; i+=3)
     {
        ImgY[i/3] = 0.299*ImgIn[i] + 0.587*ImgIn[i+1] + 0.114*ImgIn[i+2];
        ImgCb[i/3] = -0.1687*ImgIn[i] -0.3313*ImgIn[i+1] + 0.5*ImgIn[i+2] + 128;
        ImgCr[i/3] = 0.5*ImgIn[i] -0.4187*ImgIn[i+1] + -0.0813*ImgIn[i+2] + 128;
     }

   ecrire_image_pgm((char*)"Y.pgm", ImgY,  nH, nW);
   ecrire_image_pgm((char*)"Cb.pgm", ImgCb,  nH, nW);
   ecrire_image_pgm((char*)"Cr.pgm", ImgCr,  nH, nW);
   free(ImgIn);
   free(ImgY);
   free(ImgCb);
   free(ImgCr);
   return 1;
}