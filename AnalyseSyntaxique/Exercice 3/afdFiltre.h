/**
 * @file afd.h
 * @brief Définition d'un AFD du langage C
 * @author Michel Meynard
 */
#define EINIT 0
#define EA 1
#define EAB 2
#define EABC 3
#define EB 4
#define EBD 5
#define NBETAT 6

int TRANS[NBETAT][256];		/* table de transition : état suivant */
int JETON[NBETAT];		/* jeton (1-32000) ou non final (0) ? */

void classe(int ed, int cd, int cf, int ef){
  int carCourant;
  for(carCourant = cd; carCourant <= cf; carCourant++){
    TRANS[ed][carCourant]=ef;
  }
}

void creerAfd(){			/* Construction de l'AFD */
  int i;int j;			/* variables locales */
  for (i=0;i<NBETAT;i++){
    for(j=0;j<256;j++) TRANS[i][j]=-1; /* init vide */
    JETON[i]=0;			/* init tous états non finaux */
  }
  /* Transitions de l'AFD */
  /*
  TRANS[EINIT]['a']=EA;TRANS[EA]['b']=EAB;TRANS[EAB]['b']=EAB;
  TRANS[EAB]['c']=EABC;TRANS[EINIT]['b']=EB;TRANS[EB]['d']=EBD; */
  classe(EINIT, 'a', 'd', EA);
  classe(EA, 'b', 'd', EABC);
  JETON[EA]=300;JETON[EABC]=301;JETON[EBD]=-302; /* jetons des états finaux */
} /* les bd sont à filtrer : commentaires */