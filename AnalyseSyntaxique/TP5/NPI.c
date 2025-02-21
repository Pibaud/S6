#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


                             /* les macros sont des blocs : pas de ';' apres */
#define AVANCER {jeton=getchar();numcar++;}
#define TEST_AVANCE(prevu) {if (jeton==(prevu)) AVANCER else ERREUR_SYNTAXE}
#define ERREUR_SYNTAXE {printf("\nMot non reconnu : erreur de syntaxe \
au caractère numéro %d, de jeton %d \n",numcar,jeton); exit(1);} 
#define INVITE "Veuillez saisir une expression numérique SVP (q pour quitter) : "

/* déclarations en avant des fonctions */
void X(char* c);int E(char* c);int R(int g, char* c);int T(char* c);int S(int g, char* c);int F(char* c);

int jeton;                       /* caractère courant du flot d'entrée */
int numcar=0;                    /* numero du caractère courant (jeton) */

void X(char* c){			/* AXIOME */
  int r;
  if (jeton==-1){     /* règle : X -> EOF */  
    exit(0);
  } else if (jeton=='q'){		/* regle : X -> 'q' '\n' */
    AVANCER;
    if (jeton=='\n')
      return;			/* OK */
    else
      ERREUR_SYNTAXE;		/* q suivi d'autre chose */
  }
  else {
    r=E(c);			/* regle : X -> E '\n' X */
    if (jeton=='\n'){
      printf("Mot reconnu de valeur : %d\n",r);
      printf(INVITE);
      numcar=0;			/* réinit */
      AVANCER;			/* avance au prochain jeton */
      X(c);			/* boucle tq pas 'q' */
    }
    else ERREUR_SYNTAXE;          /* expression reconnue mais reste des car */
  }
}
int E(char* c){                       	/* regle : E->TR */
  return R(T(c), c);		
}
int R(int g, char* c){
  if (jeton=='+') {             /* regle : R->+TR */
    AVANCER;
    int t = T(c);
    strcat(c, "+");
    int v = R(g + t, c);        /* associativité à gauche */
    return v;
  }
  else{                          /* regle : R->epsilon */
    return g;                   /* ret la partie gauche */
  }
}
int T(char* c){                 /* regle : T->FS */
  return S(F(c), c);
}
int S(int g, char* c){
  if (jeton=='*') {             /* regle : S->*FS */
    AVANCER;
    int f = F(c);
    strcat(c, "*");
    int v = S(g * f, c);        /* associativité à gauche */
    return v;
  }
  else {                        /* regle : S->epsilon */
    return g;                   /* ret la partie gauche */
  }
}
int F(char* c){
  int r;
  if (jeton=='(') {             /* regle : F->(E) */
    AVANCER;
    r = E(c);                    /* save the result of E() in r */
    TEST_AVANCE(')');
  }
  else 
    if (isdigit(jeton)) {       /* regle : F->0|1|...|9 */
      r = jeton - '0';              /* valeur comprise entre 0 et 9 */
      char digit[2] = {jeton, '\0'};
      strcat(c, digit);
      AVANCER;
    }
    else ERREUR_SYNTAXE;
  return r;
}
int main(){                         /* Fonction principale */
  char npi[100];
  printf(INVITE);
  numcar=0;			/* init */
  AVANCER;                      /* initialiser jeton sur le premier car */
  X(npi);				/* axiome */
  printf("NPI : %s\n", npi);
  return 0;			/* ne retourne jamais que par X */
}