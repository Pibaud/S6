#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

/* Programme serveur */

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

int recvTCP(int sock, void *msg, int sizeMsg)
{
  int toRecv = sizeMsg;
  int received = 0;
  while (toRecv > received)
  {
    int res = recv(sock, msg + received, toRecv - received, 0);
    if (res == -1)
    {
      return -1;
    }
    if (res == 0)
    {
      return received;
    }
    received += res;
  }
  return received;
}

int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */
  int ds = socket(PF_INET, SOCK_STREAM, 0);

  if (ds == -1)
  {
    perror("Serveur : pb creation socket :");
    exit(1);
  }

  printf("Serveur : creation de la socket réussie \n");

  /* Etape 2 : Nommer la socket du serveur */
  struct sockaddr_in adrServ;
  adrServ.sin_family = AF_INET;
  adrServ.sin_port = htons(atoi(argv[1]));
  adrServ.sin_addr.s_addr = INADDR_ANY;

  if (bind(ds, (struct sockaddr *)&adrServ, sizeof(adrServ)) == -1)
  {
    perror("Serveur : erreur bind");
    close(ds);
    exit(1);
  }

  if (listen(ds, 10) == -1)
  {
    perror("Serveur : erreur listen");
    close(ds);
    exit(1);
  }

  printf("Serveur : serveur en attente de connexion...\n");

  /* Etape 4 : recevoir un message du client (voir sujet pour plus de détails)*/

  char msg[4000];
  struct sockaddr_in structClient;
  socklen_t addr_len = sizeof(structClient);

  while (1)
  {
    printf("itération\n");
    int dsClient = accept(ds, (struct sockaddr *)&structClient, &addr_len);

    if (dsClient == -1)
    {
      perror("Serveur : erreur accept");
      close(ds);
      exit(1);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
      perror("Erreur de fork");
      close(dsClient);
      continue;
    }

    if (pid == 0)
    {
      close(ds);
      int tailleMsg = recvTCP(dsClient, msg, sizeof(int));

      printf("Serveur : taille du message reçu : %d\n", tailleMsg);

      int res = recvTCP(dsClient, msg, tailleMsg);

      if (res <= 0)
      {
        perror("Serveur : erreur reception message");
        close(ds);
        exit(1);
      }

      printf("Serveur : le message reçu de %s est : %s\n", inet_ntoa(structClient.sin_addr), msg);

      /* Etape 5 : envoyer au client le nombre d'octets reçus (voir sujet pour plus de détails)*/

      if (sendTCP(dsClient, &res, sizeof(int)) <= 0)
      {
        perror("Serveur : erreur envoi message");
        close(dsClient);
        close(ds);
        exit(1);
      }

      /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/

      close(dsClient);
      exit(0);
    }
    else
    {
      close(dsClient);
    }
  }
  close(ds);

  printf("Serveur : je termine\n");
  return 0;
}
