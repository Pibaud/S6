#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int recvTCP(int sock, void *msg, int sizeMsg) {
  while (sizeMsg > 0) {
    int rcv = recv(sock, msg, sizeMsg, 0);
    if (rcv < 0) {
      return -1;
    }
    if (rcv == 0) {
      return 0;
    }
    msg = (char *)msg + rcv;
    sizeMsg -= rcv;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  signal(SIGPIPE, SIG_IGN); // Ignorer SIGPIPE pour éviter les plantages

  if (argc != 2) {
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }

  int ds = socket(PF_INET, SOCK_STREAM, 0);
  if (ds == -1) {
    perror("Serveur : problème création socket");
    exit(1);
  }
  printf("Serveur : création de la socket réussie\n");

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[1]));
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  socklen_t tailleStructServ = sizeof(serverAddr);
  if (bind(ds, (struct sockaddr *)&serverAddr, tailleStructServ) < 0) {
    perror("Serveur : erreur lors du bind");
    close(ds);
    exit(1);
  }
  printf("Serveur : bind réussi\n");

  if (listen(ds, 10) < 0) {
    perror("Erreur de listen");
    close(ds);
    exit(1);
  }
  printf("Serveur : en attente de connexions\n");

  while (1) {
    struct sockaddr_in clientAddr;
    socklen_t tailleStructClient = sizeof(clientAddr);

    int dsClient =
        accept(ds, (struct sockaddr *)&clientAddr, &tailleStructClient);
    if (dsClient < 0) {
      perror("Erreur de accept");
      continue;
    }

    printf("Serveur : connexion acceptée\n");

    pid_t pid = fork();
    if (pid < 0) {
      perror("Erreur de fork");
      close(dsClient);
      continue;
    }

    if (pid == 0) { // Processus enfant
      close(ds);    // L'enfant n'a pas besoin du socket serveur
      char msgCli[132] = {0};
      int rcvStatus = recvTCP(dsClient, msgCli, sizeof(msgCli));

      if (rcvStatus == 1) {
        printf("Reçu du client : %s\n", msgCli);
      } else if (rcvStatus == 0) {
        printf("Le client s'est déconnecté proprement\n");
      } else {
        printf("Erreur lors de la réception des données\n");
      }
      close(dsClient); // Fermeture du socket client dans l'enfant
      exit(0);         // Terminer l'enfant
    } else {
      // Processus parent
      close(dsClient); // Le parent n'a plus besoin du socket client
    }
  }

  close(ds);
  return 0;
}