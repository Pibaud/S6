#include <iostream>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "calcul.h"

struct predicatRdv {
  int cpt;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
};

struct params {
  int idThread;
  struct predicatRdv * varPartagee;
};

void * participant (void * p){ 

  struct params * args = (struct params *) p;
  struct predicatRdv * predicat = args -> varPartagee;
  pthread_mutex_lock(&predicat->mutex);
  predicat->cpt--;
  if(predicat->cpt == 0){
    pthread_cond_broadcast(&predicat->cond);
  }
  
  // simulation d'un long calcul pour le travail avant RdV
  calcul (args -> idThread + rand() % 2 +1); // c'est un exemple.

  printf("Thread %d : j'ai fini mon premier job\n", args->idThread);

  // RdV 
  while (predicat->cpt > 0) {  // attention : le dernier arriv� ne doit pas attendre. Il doit r�veiller tous les autres.
    pthread_cond_wait(&predicat->cond, &predicat->mutex);
  }

  pthread_mutex_unlock(&predicat->mutex);

  printf("Thread %d : RdV atteint\n", args->idThread);

  calcul(args->idThread + rand() % 2 +1); // reprise et poursuite de l'execution.

  printf("Thread %d : j'ai fini mon blowjob\n", args->idThread);

  pthread_exit(NULL);
}

int main(int argc, char * argv[]){
  
  if (argc != 2) {
    std::cout << "argument requis" << std::endl;
    std::cout << "./prog nombre_Threads" << std::endl;
    exit(1);
  }
 
  // initialisations 
  pthread_t threads[atoi(argv[1])];
  struct params tabParams[atoi(argv[1])];
  struct predicatRdv predicatRdv;
  predicatRdv.cpt = atoi(argv[1]);
  pthread_mutex_init(&predicatRdv.mutex, NULL);
  pthread_cond_init(&predicatRdv.cond, NULL);

  srand(atoi(argv[1]));  // initialisation de rand pour la simulation de longs calculs
 
  // création des threads 
  for (int i = 0; i < atoi(argv[1]); i++){
    tabParams[i].idThread = i+1;
    tabParams[i].varPartagee = &predicatRdv;

    if (pthread_create(&threads[i], NULL, participant, &tabParams[i]) != 0){
      perror("erreur creation thread");
      exit(1);
    }
  }

  // attente de la fin des  threards. Partie obligatoire 
  for (int i = 0; i < atoi(argv[1]); i++){
      pthread_join(threads[i], NULL);
    }
  std::cout << "thread principal : fin de tous les threads secondaires" << std::endl;

  // terminer "proprement". 
  pthread_mutex_destroy(&predicatRdv.mutex);
  pthread_cond_destroy(&predicatRdv.cond);
  return 0;
}