// test_couleur.cpp : Seuille une image couleur selon 3 seuils

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], nomFichierDat[250];
  int nH, nW, nTaille, nR, nG, nB ;
  int histor[256],histog[256],histob[256];//Ce sera nos histogrammes
 for (int i=0;i<256;i++){
        histor[i]=0;
        histog[i]=0;
        histob[i]=0;

    }
  if (argc != 3) 
     {
       printf("Usage: ImageIn.ppm out.dat\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",nomFichierDat);
 

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);
	
   for (int i=0; i < nTaille3; i++)
     {
      switch (i%3){
      case 0:
        histor[ImgIn[i]]++;
      break;
      case 1:
        histog[ImgIn[i]]++;

      break;
      
      default:
        histob[ImgIn[i]]++;

      break;
      }
     }

   free(ImgIn);
  FILE *file = fopen(nomFichierDat, "w");
        if (file == NULL) {
            perror("Impossible d'ouvrir le fichier dat sortie\n");
            return 1;
    }


    for (int i = 1; i < 256; i++) { 
        fprintf(file, "%d %d %d %d\n", i, histor[i], histob[i], histog[i]);
    }


   
    fclose(file);
   return 1;
}

//Commande à faire à la fin : 
// gnuplot> plot "histogrammecouleur.dat" using 1:2 w l, "" using 1:3 w l, "" using 1:4 w l
