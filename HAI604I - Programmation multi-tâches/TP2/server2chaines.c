#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }

  int ds = socket(PF_INET, SOCK_STREAM, 0);
  if (ds == -1) {
    perror("Serveur : pb creation socket :");
    exit(1);
  }
  printf("Serveur : création de la socket réussie \n");

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[1]));
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  socklen_t tailleStructServ = sizeof(serverAddr);
  if (bind(ds, (struct sockaddr *)&serverAddr, tailleStructServ) < 0) {
    perror("Serveur : erreur lors du bind :");
    close(ds);
    exit(1);
  }
  printf("Serveur : bind réussi\n");

  if (listen(ds, 10) < 0) {
    perror("erreur de listen");
  }

  struct sockaddr_in clientAddr;
  socklen_t tailleStructClient = sizeof(clientAddr);

  int dsClient =
      accept(ds, (struct sockaddr *)&clientAddr, &tailleStructClient);

  if (dsClient < 0) {
    perror("erreur de accept");
  }

  char msgCli[4000] = {0};

  int nbrcv = 0;
  int nboc = 0;

  char c;
  printf("Entrez un char pour poursuivre");
  scanf("%c", &c);

  while (1) {
    int rcv = recv(dsClient, msgCli, sizeof(msgCli), 0);
  if (rcv < 0) {
    perror("Serveur : erreur recv");
    break;
  } else if (rcv == 0) {
    printf("Serveur : connexion fermée par le client\n");
    break;
  }
    nboc += rcv;
    nbrcv ++;
    msgCli[rcv] = '\0';
    printf("Serveur : (appel n°%d à recv) reçu '%s' (%d octets depuis le début) de %s:%d\n", nbrcv, msgCli, nboc, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    int resp = strlen(msgCli);
    if (send(dsClient, &resp, sizeof(int), 0) < 0) {
      perror("Serveur : erreur envoi réponse :");
    } else {
      printf("Serveur : réponse envoyée : %d octets\n", resp);
    }
  }
  close(ds);
  printf("Serveur : je termine\n");
  return 0;
}