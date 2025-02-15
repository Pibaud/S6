#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_BUFFER_SIZE 146980

int recvTCP(int sock, void *msg, int sizeMsg)
{
  while (sizeMsg > 0)
  {
    int rcv = recv(sock, msg, sizeMsg, 0);
    if (rcv < 0)
    {
      return -1;
    }
    if (rcv == 0)
    {
      return 0;
    }
    msg = (char *)msg + rcv;
    sizeMsg -= rcv;
  }
  return 1;
}

int main(int argc, char *argv[])
{
  signal(SIGPIPE, SIG_IGN);

  if (argc != 2)
  {
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }

  /* etape 1 : creer une socket d'écoute */
  int ds = socket(PF_INET, SOCK_STREAM, 0);
  if (ds == -1)
  {
    perror("Serveur : problème création socket");
    exit(1);
  }
  printf("Serveur : création de la socket réussie\n");

  /* activer l'option de réutilisation d'adresse */
  int opt = 1;
  if (setsockopt(ds, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
  {
    perror("Serveur : setsockopt failed");
    close(ds);
    exit(1);
  }

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[1]));
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  /* etape 2 : nommage de la socket */
  socklen_t tailleStructServ = sizeof(serverAddr);
  if (bind(ds, (struct sockaddr *)&serverAddr, tailleStructServ) < 0)
  {
    perror("Serveur : erreur lors du bind");
    close(ds);
    exit(1);
  }
  printf("Serveur : bind réussi\n");

  /* etape 3 : mise en ecoute */
  if (listen(ds, 10) < 0)
  {
    perror("Erreur de listen");
    close(ds);
    exit(1);
  }

  printf("Serveur : en attente de connexions\n");

  while (1)
  {
    int totalRecv = 0;

    struct sockaddr_in clientAddr;
    socklen_t tailleStructClient = sizeof(clientAddr);

    int dsClient = accept(ds, (struct sockaddr *)&clientAddr, &tailleStructClient);
    if (dsClient < 0)
    {
      perror("Erreur de accept");
      continue;
    }

    printf("Serveur : connexion acceptée\n");

    /* Recevoir et traiter le fichier */
    const char *filepath = "../reception/";

    int filename_len;
    if (recvTCP(dsClient, &filename_len, sizeof(int)) <= 0) {
      perror("Serveur : erreur réception taille nom fichier");
      close(dsClient);
      continue;
    }
    totalRecv += sizeof(int);
    
    printf("Serveur : taille du nom du fichier de %d octets reçue\n", filename_len);

    char *filename = malloc(filename_len);
    if (filename == NULL) {
      perror("Serveur : erreur allocation mémoire");
      close(dsClient);
      continue;
    }

    if (recvTCP(dsClient, filename, filename_len) <= 0) {
      perror("Serveur : erreur réception nom fichier");
      free(filename);
      close(dsClient);
      continue;
    }
    totalRecv += filename_len;
    
    printf("Serveur : nom de fichier : '%s' bien reçu\n", filename);

    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "%s%s", filepath, filename);

    int tailleFichier;
    if (recvTCP(dsClient, &tailleFichier, sizeof(int)) <= 0) {
      perror("Serveur : erreur réception taille fichier");
      free(filename);
      close(dsClient);
      continue;
    }
    totalRecv += sizeof(int);
    
    printf("Serveur : taille du fichier de %d octets reçue\n", tailleFichier);

    // Ouvrir le fichier en écriture
    FILE *file = fopen(fullpath, "wb");
    if (file == NULL)
    {
      perror("Serveur : erreur ouverture fichier");
      free(filename);
      close(dsClient);
      continue;
    }

    // Recevoir et écrire les données du fichier
    char buffer[MAX_BUFFER_SIZE];
    int totalWritten = 0;
    
    while (totalWritten < tailleFichier)
    {
      int toRead = tailleFichier - totalWritten;
      if (toRead > MAX_BUFFER_SIZE)
        toRead = MAX_BUFFER_SIZE;
      
      int received = recv(dsClient, buffer, toRead, 0);
      if (received <= 0)
      {
        perror("Serveur : erreur réception données fichier");
        break;
      }
      
      size_t written = fwrite(buffer, 1, received, file);
      if (written < received)
      {
        perror("Serveur : erreur écriture fichier");
        break;
      }
      
      totalWritten += written;
      totalRecv += received;
    }

    printf("Serveur : %d octets reçus et écrits dans le fichier %s\n", 
           totalWritten, fullpath);
    
    fclose(file);
    free(filename);
    close(dsClient);
    
    printf("Serveur : traitement terminé, en attente d'une nouvelle connexion\n");
  }

  close(ds);
  return 0;
}