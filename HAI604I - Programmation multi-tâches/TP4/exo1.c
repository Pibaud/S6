#include <string.h>
#include <stdio.h>//perror
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "calcul.h"

pthread_mutex_t mutex;

struct paramsFonctionThread {
  int idThread;
  int *a;
};

void * fonctionThread (void * params){
  struct paramsFonctionThread * args = (struct paramsFonctionThread *) params;
  printf("Le thread %d commence\n", args->idThread);
  calcul(1);

  pthread_mutex_lock(&mutex);
  *(args->a) += 1;
  printf("a = %d\n", *(args->a));
  pthread_mutex_unlock(&mutex);

  printf("Le thread %d termine\n", args->idThread);
}

int main(int argc, char * argv[]){

  if (pthread_mutex_init(&mutex, NULL) != 0) {
    perror("pthread_mutex_init");
    exit(EXIT_FAILURE);
  }

  if (argc < 2 ){
    printf("utilisation: %s  nombre_threads  \n", argv[0]);
    return 1;
  }
  
  pthread_t threads[atoi(argv[1])];
  struct paramsFonctionThread structuresParams[atoi(argv[1])];
  int var = 12;

  for (int i = 0; i < atoi(argv[1]); i++){

    structuresParams[i].idThread = i;
    structuresParams[i].a = &var;

    if (pthread_create(&threads[i], NULL, fonctionThread, &structuresParams[i]) != 0){
      perror("erreur creation thread");
      exit(1);
    }
  }

  printf("saisir un caractère \n");
  fgetc(stdin);

  return 0;
}