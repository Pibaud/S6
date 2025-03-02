#include <stdio.h> //perror
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> //close
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // pour mkdir

#define MAX_BUFFER_SIZE 146980

int main(int argc, char *argv[])
{
  /* etape 0 : gestion des paramètres si vous souhaitez en passer */
  if (argc != 2)
  {
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(0);
  }
  /* etape 1 : creer une socket d'écoute des demandes de connexions*/
  int ds = socket(PF_INET, SOCK_STREAM, 0);
  if (ds == -1)
  {
    perror("Serveur : erreur creation socket\n");
    exit(1);
  }
  printf("Serveur : creation de la socket : ok\n");
  /* etape 2 : nommage de la socket */

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(atoi(argv[1]));

  if (bind(ds, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Serveur : erreur au bind\n");
    exit(1);
  }

  /* etape 3 : mise en ecoute des demandes de connexions */

  if (listen(ds, 5) == -1)
  {
    perror("Serveur : erreur au listen\n");
    exit(1);
  }
  printf("Serveur : mise en écoute : ok\n");

  /* etape 4 : plus qu'a attendre la demande d'un client */
  struct sockaddr_in client_addr;
  socklen_t lgAddr = sizeof(struct sockaddr_in);
  int cptClient = 0;
  while (1)
  {
    int dsClt = accept(ds, (struct sockaddr *)&client_addr, &lgAddr);
    if (dsClt == -1)
    {
      perror("Serveur : erreur accept\n");
      exit(1);
    }
    printf("Serveur : connexion établie\n");
    cptClient++;
    pid_t pid = fork();
    if (pid == -1)
    {
      perror("Serveur : erreur fork\n");
      exit(1);
    }
    if (pid == 0)
    {
      close(ds);
      long long int totalRecv = 0; // un compteur du nombre total d'octets recus d'un client spécifique
      long long int nbRecv = 0;    // un compteur du nombre d'appels à recv pour un client spécifique
      char buffer[MAX_BUFFER_SIZE];

      // Réception de la taille du fichier
      long long int fileSize;
      long long int recvSize = recv(dsClt, &fileSize, sizeof(long long int), 0);
      if (recvSize < 0)
      {
        perror("Serveur : erreur reception taille fichier\n");
        close(dsClt);
        exit(1);
      }
      totalRecv += recvSize;
      nbRecv++;
      printf("Serveur : j'ai reçu la taille du fichier : %lld octets\n", fileSize);

      // Réception de la taille du nom du fichier
      size_t fileNameSize;
      recvSize = recv(dsClt, &fileNameSize, sizeof(size_t), 0);
      if (recvSize < 0)
      {
        perror("Serveur : erreur reception taille nom fichier\n");
        close(dsClt);
        exit(1);
      }
      totalRecv += recvSize;
      nbRecv++;
      printf("Serveur : j'ai reçu la taille du nom du fichier : %ld octets\n", fileNameSize);

      char *filename = malloc(fileNameSize);

      // Réception du nom du fichier
      recvSize = recv(dsClt, filename, fileNameSize, 0);
      if (recvSize < 0)
      {
        perror("Serveur : erreur reception nom fichier\n");
        close(dsClt);
        exit(1);
      }
      totalRecv += recvSize;
      nbRecv++;
      printf("Serveur : j'ai reçu le nom du fichier : %s\n", filename);

      // Création du chemin du fichier
      char *filepath = malloc(30 + fileNameSize + 1);
      filepath[0] = '\0';
      //faire un dossier portant la valeur de cptClient
      char clientDir[50];
      snprintf(clientDir, sizeof(clientDir), "../reception/client_%d/", cptClient);
      
      strcat(filepath, clientDir);
      strcat(filepath, filename);   

      mkdir(clientDir, 0755);

      // On ouvre le fichier dans lequel on va écrire
      FILE *file = fopen(filepath, "wb");
      if (file == NULL)
      {

        perror("Serveur : erreur ouverture fichier: \n");
        printf("(Voulait ouvrir %s)\n", filepath);
        free(filename);
        free(filepath);
        close(dsClt);
        exit(1);
      }

      // Réception du contenu du fichier
      long long int totalRead = 0;

      while (totalRead < fileSize)
      {
        // réception de la taille du bloc à lire
        long long int blockSize;
        recvSize = recv(dsClt, &blockSize, sizeof(long long int), 0);
        if (recvSize < 0)
        {
          perror("Serveur : erreur reception taille bloc\n");
          fclose(file);
          close(dsClt);
          exit(1);
        }
        totalRecv += recvSize;
        nbRecv++;
        printf("Serveur : j'ai reçu la taille du bloc : %lld octets\n", blockSize);

        long long int readInBlock = 0;
        while (readInBlock < blockSize)
        {
          recvSize = recv(dsClt, buffer + readInBlock, blockSize - readInBlock, 0);
          if (recvSize < 0)
          {
            perror("Serveur : erreur reception bloc\n");
            fclose(file);
            close(dsClt);
            exit(1);
          }
          totalRecv += recvSize;
          nbRecv++;
          readInBlock += recvSize;
          printf("Serveur : j'ai reçu un bloc de %lld octets\n", recvSize);
          // écriture dans le fichier
          long long int totalWritten = 0;
          while (totalWritten < recvSize)
          {
            size_t written = fwrite(buffer + totalWritten, sizeof(char), recvSize - totalWritten, file);
            totalWritten += written;
          }
          printf("Serveur : j'ai écrit un bloc de %lld octets\n", totalWritten);
        }
        totalRead += readInBlock;
      }

      printf("Serveur : ecriture dans fichier reussie. Vous pouvez vérifier la création du fichier et son contenu.\n");
      // fermeture du fichier
      fclose(file);
      free(filename);
      free(filepath);
      printf("Serveur : j'ai reçu au total : %lld octets avec ce client \n", totalRecv);
      printf("Serveur : nombre total d'appels recv avec ce client : %lld\n", nbRecv);

      totalRecv = 0;
      nbRecv = 0;
      close(dsClt);

      printf("Serveur : connexion avec le client fermée.\n");
    }
    else
    {
      close(dsClt);
    }
  }
  close(ds);
  printf("Serveur : c'est fini\n");
}