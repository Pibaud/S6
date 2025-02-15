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

int sendTCP(int sock, void *msg, int sizeMsg)
{
  while (sizeMsg != 0)
  {
    int sent = send(sock, msg, sizeMsg, 0);
    if (sent == -1)
    {
      return -1;
    }
    if (sent == 0)
    {
      return 0;
    }
    msg = (char *)msg + sent;
    sizeMsg -= sent;
  }
  return 1;
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    printf("utilisation : client ip_serveur port_serveur nom_fichier\n Remaque : le parametre nom_fichier correspond au nom d'un fichier existant dans le répertoire emission\n");
    exit(0);
  }

  /* etape 1 : créer une socket */
  int ds = socket(PF_INET, SOCK_STREAM, 0);
  if (ds == -1) {
    perror("Client : erreur création socket");
    exit(1);
  }

  /* etape 2 : designer la socket du serveur */
  struct sockaddr_in server_addr;
  server_addr.sin_family = PF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  /* etape 3 : demander une connexion */
  socklen_t server_len = sizeof(server_addr);
  if (connect(ds, (struct sockaddr *)&server_addr, server_len) < 0)
  {
    perror("erreur de connect");
    close(ds);
    exit(1);
  }

  /* etape 4 : envoi de fichier */
  int totalSent = 0;

  // construction du chemin vers le fichier
  char filepath[512];
  sprintf(filepath, "../emission/%s", argv[3]);
  
  char *filename = argv[3];
  int filename_len = strlen(filename) + 1; // Inclure le caractère nul
  
  printf("Client : J'envoie la taille du nom du fichier : %s : %d octets\n", filename, filename_len);
  if (sendTCP(ds, &filename_len, sizeof(int)) < 0) { // Envoyer la taille du nom de fichier
    perror("Client : erreur envoi taille nom fichier");
    close(ds);
    exit(1);
  }
  
  if (sendTCP(ds, filename, filename_len) < 0) {     // Envoyer le nom de fichier
    perror("Client : erreur envoi nom fichier");
    close(ds);
    exit(1);
  }
  totalSent += sizeof(int) + filename_len;
  
  printf("Client : J'envoie %s\n", filename);

  // obtenir la taille du fichier
  struct stat attributes;
  if (stat(filepath, &attributes) == -1)
  {
    perror("Client : erreur stat");
    close(ds);
    exit(1);
  }

  int file_size = attributes.st_size;
  if (sendTCP(ds, &file_size, sizeof(int)) < 0) {
    perror("Client : erreur envoi taille fichier");
    close(ds);
    exit(1);
  }
  totalSent += sizeof(int);

  printf("Client : taille du fichier envoyée : %d octets\n", file_size);

  // lecture du contenu d'un fichier
  FILE *file = fopen(filepath, "rb");
  if (file == NULL)
  {
    perror("Client : erreur ouverture fichier \n");
    close(ds);
    exit(1);
  }

  int total_lu = 0;
  char buffer[MAX_BUFFER_SIZE];
  while (total_lu < file_size)
  {
    size_t read = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, file);
    if (read == 0)
    {
      if (ferror(file) != 0)
      {
        perror("Client : erreur lors de la lecture du fichier \n");
        fclose(file);
        close(ds);
        exit(1);
      }
      else
      {
        printf("Client : arrivé a la fin du la lecture du fichier\n");
        break;
      }
    }
    
    // Envoyer le bloc lu
    if (sendTCP(ds, buffer, read) < 0) {
      perror("Client : erreur envoi données fichier");
      fclose(file);
      close(ds);
      exit(1);
    }
    totalSent += read;
    
    printf("Client : j'ai lu et envoyé un bloc du fichier de %zu octets\n", read);
    total_lu += read;
  }

  fclose(file);
  close(ds);

  printf("Client : j'ai lu au total : %d octets \n", total_lu);
  printf("Client : j'ai envoyé au total : %d octets \n", totalSent);
  printf("Client : c'est fini\n");
  
  return 0;
}