#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_BUFFER_SIZE 146980

int main(int argc, char *argv[])
{

  if (argc != 4)
  {
    printf("utilisation : client ip_serveur port_serveur nom_fichier\n Remaque : le parametre nom_fichier correspond au nom d'un fichier existant dans le répertoire emission\n");
    exit(0);
  }
  long long int nbAppelsSend = 0;
  long long int nbOctetsEnvoyes = 0;

  /* etape 1 : créer une socket */

  int ds = socket(PF_INET, SOCK_STREAM, 0);

  /* etape 2 : designer la socket du serveur */

  struct sockaddr_in adrServ;
  adrServ.sin_family = AF_INET;
  adrServ.sin_port = htons(atoi(argv[2]));      // conversion en format réseau
  adrServ.sin_addr.s_addr = inet_addr(argv[1]); // conversion en format réseau
  socklen_t lgAdr = sizeof(struct sockaddr_in);

  /* etape 3 : demander une connexion */

  if (connect(ds, (struct sockaddr *)&adrServ, lgAdr) == -1){
    perror("Client : erreur de connexion");
    close(ds);
    exit(1);
  }

  /* etape 4 : envoi de fichier : attention la question est générale. Il faut creuser pour définir un protocole d'échange entre le client et le serveur pour envoyer correctement un fichier */

  /* le bout de code suivant est une lecture de contenu d'un fichier dont le nom est passé en paramètre.
     - pour lire un fichier, il faut l'ouvrir en mode lecture
     - la lecture se fait par blocs d'octets jusqu'à la fin du fichier.
     - la taille d'un bloc est définie par la constante MAX_BUFFER_SIZE que vous pouvez modifier.

     Le code est à compléter pour mettre en place l'envoi d'un fichier.
  */

  // construction du nom du chemin vers le fichier
  char *filepath = malloc(strlen(argv[3]) + 16); // ./emission/+nom fichier +\0
  filepath[0] = '\0';
  strcat(filepath, "../emission/");
  char *filename = argv[3];
  strcat(filepath, filename);

  printf("Client: je vais envoyer %s\n", filepath);

  // obtenir la taille du fichier
  struct stat attributes;
  if (stat(filepath, &attributes) == -1)
  {
    perror("Client : erreur stat");
    free(filepath);
    exit(1);
  }

  long long int file_size = attributes.st_size; // cette copie est uniquement informer d'où obtenir la taille du fichier.

  printf("Client : taille du fichier : %lld octets\n", file_size);

  // envoi de la taille du fichier
  long long int sentSize = send(ds, &file_size, sizeof(long long int), 0);
  if (sentSize < 0){
    perror("Client : erreur envoi taille fichier");
    free(filepath);
    exit(1);
  }
  nbAppelsSend++;
  nbOctetsEnvoyes += sentSize;
  printf("Client : j'envoie la taille du fichier : %lld octets\n", file_size);

  //envoi de la taille du nom du fichier
  long long int filename_len = strlen(filename) + 1; // Inclure le caractère nul
  sentSize = send(ds, &filename_len, sizeof(long long int), 0);
  if (sentSize < 0) {
    perror("Client : erreur envoi taille nom fichier");
    free(filepath);
    exit(1);
  }
  nbAppelsSend++;
  nbOctetsEnvoyes += sentSize;
  printf("Client : j'envoie la taille du nom du fichier : %s : %lld octets\n", filename, filename_len);

  //envoi du nom du fichier
  sentSize = send(ds, filename, filename_len, 0);
  if (sentSize < 0) {
    perror("Client : erreur envoi nom fichier");
    free(filepath);
    exit(1);
  }
  nbAppelsSend++;
  nbOctetsEnvoyes += sentSize;
  printf("Client : nom du fichier envoyé : %s\n", filename);
  
  // lecture du contenu d'un fichier et envoi
  FILE *file = fopen(filepath, "rb");
  if (file == NULL){
    perror("Client : erreur ouverture fichier \n");
    free(filepath);
    exit(1);
  }
  free(filepath);

  long long int total_lu = 0;
  char buffer[MAX_BUFFER_SIZE];
  while (total_lu < file_size){
    long long int read = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, file);
    if (read == 0){
      if (ferror(file) != 0){
        perror("Client : erreur lors de la lecture du fichier \n");
        fclose(file);
        exit(1);
      }
      else {
        printf("Client : arrivé a la fin de la lecture du fichier\n"); // fin du fichier
        break;
      }
    }
    printf("Client : j'ai lu un bloc du fichier \n");
    total_lu += read;

    //envoi de la taille du bloc lu
    sentSize = send(ds, &read, sizeof(size_t), 0);
    if (sentSize < 0){
      perror("Client : erreur envoi taille bloc fichier");
      fclose(file);
      exit(1);
    }
    nbAppelsSend++;
    nbOctetsEnvoyes += sentSize;
    printf("Client : j'envoie la taille du bloc lu : %lld octets\n", read);

    //envoi du bloc lu
    long long int readSent = 0;
    while (readSent < read) {
      long long int sent = send(ds, buffer + readSent, read - readSent, 0);
      if (sent < 0) {
        perror("Client : erreur envoi fichier");
        fclose(file);
        exit(1);
      }
      nbAppelsSend++;
      nbOctetsEnvoyes += sent;
      readSent += sent;
    }
    printf("Client : j'ai envoyé un bloc du fichier de taille : %lld octets\n", read);
  }

  // fermeture du fichier
  fclose(file);
  close(ds);
  printf("Client : j'ai lu au total : %lld octets \n", total_lu);
  printf("Client : nombre total d'appels send : %lld\n", nbAppelsSend);
  printf("Client : nombre total d'octets envoyés : %lld\n", nbOctetsEnvoyes);
  printf("Client : c'est fini\n");
  return 0;
}
