/** @file evaldesc.c
 *@author Michel Meynard
 *@brief Evaluation descendante récursive d'expression arithmétique.
 *Chaque expression arithmétiques est composée de littéraux entiers sur un car, 
 * des opérateurs +, * et du parenthésage (). On utilise l'associativité à 
 * gauche : 1+2+3=(1+2)+3; 1*2*3=(1*2)*3. Soit rediriger en entrée un fichier, 
 * soit terminer par deux caractères EOF (Ctrl-D), un pour lancer la lecture,
 * l'autre comme "vrai" EOF.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

                             /* les macros sont des blocs : pas de ';' apres */
#define AVANCER {jeton=getchar();numcar++;}
#define TEST_AVANCE(prevu) {if (jeton==(prevu)) AVANCER else ERREUR_SYNTAXE}
#define ERREUR_SYNTAXE {printf("\nMot non reconnu : erreur de syntaxe \
au caractère numéro %d, de jeton %d \n",numcar,jeton); exit(1);} 
#define INVITE "Veuillez saisir une expression numérique SVP (q pour quitter) : "

/* déclarations en avant des fonctions */
void X();int E();int R(int g);int T();int S(int g);int F();

int jeton;                       /* caractère courant du flot d'entrée */
int numcar=0;                    /* numero du caractère courant (jeton) */

void X(){			/* AXIOME */
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
    r=E();			/* regle : X -> E '\n' X */
    if (jeton=='\n'){
      printf("Mot reconnu de valeur : %d\n",r);
      printf(INVITE);
      numcar=0;			/* réinit */
      AVANCER;			/* avance au prochain jeton */
      X();			/* boucle tq pas 'q' */
    }
    else ERREUR_SYNTAXE;          /* expression reconnue mais reste des car */
  }
}
int E(){                       	/* regle : E->TR */
  return R(T());		
}
int R(int g){
  if (jeton=='+') {             /* regle : R->+TR */
    AVANCER;
    return R(g+T());		/* associativité à gauche */
				/* return g+R(T()); asso à droite */
  }
  else                          /* regle : R->epsilon */
    return g;                   /* ret la partie gauche */
}
int T(){                      	/* regle : T->FS */
  return S(F());
}
int S(int g){
  if (jeton=='*') {             /* regle : S->*FS */
    AVANCER;
    return S(g*F());		/* associativité à gauche */
				/* return g*S(F()); asso à droite */
  }
  else                          /* regle : S->epsilon */
    return g;                   /* ret la partie gauche */
}
int F(){
  int r;                        /* resultat */
  if (jeton=='(') {             /* regle : F->(E) */
    AVANCER;
    r=E();
    TEST_AVANCE(')');
  }
  else 
    if (isdigit(jeton)) {       /* regle : F->0|1|...|9 */
      r=jeton-'0';              /* valeur comprise entre 0 et 9 */
      AVANCER;
    }
    else ERREUR_SYNTAXE;
  return r;
}
int main(){                         /* Fonction principale */
  printf(INVITE);
  numcar=0;			/* init */
  AVANCER;                      /* initialiser jeton sur le premier car */
  X();				/* axiome */
  return 0;			/* ne retourne jamais que par X */
}
