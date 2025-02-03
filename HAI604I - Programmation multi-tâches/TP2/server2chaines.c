#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]) {

  signal(SIGPIPE, SIG_IGN);

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

  char msgCli[132] = {0};

  int nbrcv = 0;
  int nboc = 0;

  char c;
  printf("Entrez un char pour poursuivre\n");
  scanf("%c", &c);

  while (1) {
    int rcv = recv(dsClient, msgCli, sizeof(msgCli), 0);
  if (rcv < 0) {
    perror("Serveur : erreur recv");
    break;
  }
  if (rcv == 0) {
    printf("Serveur : connexion fermée par le client\n");
    break;
  }
    nboc += rcv;
    nbrcv ++;
    msgCli[rcv] = '\0';
  }
  printf("Serveur : je termine avec %d octets reçus avec %d appels à recv\n",nboc, nbrcv);
  close(ds);
  return 0;
}