#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/* Programme client */

int sendTCP(int sock, void *msg, int sizeMsg) {
  while (sizeMsg != 0) {
    int sent = send(sock, msg, sizeMsg, 0);
    if (sent == -1) {
      return -1;
    }
    if (sent == 0) {
      return 0;
    }
    msg = (char *)msg + sent;
    sizeMsg -= sent;
  }
  return 1;
}
int main(int argc, char *argv[]) {

  /* je passe en paramètre l'adresse de la socket du serveur (IP et
     numéro de port) et un numéro de port à donner à la socket créée plus
     loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     paramètres sont à adapter en fonction des besoins. Sans ces
     paramètres, l'exécution doit être arrétée, autrement, elle
     aboutira à des erreurs.*/
  if (argc != 4) {
    printf("utilisation : %s ip_serveur port_serveur port_client ", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */
  int ds = socket(PF_INET, SOCK_STREAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1) {
    perror("Client : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
             // dépendent de la réussite de la création de la socket.
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = PF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  socklen_t server_len = sizeof(server_addr);

  if (connect(ds, (struct sockaddr *)&server_addr, server_len) < 0) {
    perror("erreur de connect");
  }

  char *msg = "aaaaa\0";

  char c;
  printf("Entrez un char pour poursuivre l'exécution ...\n");
  scanf("%c", &c);

  sendTCP(ds, msg, strlen(msg));

  printf("sendTCP réussi avec msg = %s\n", msg);

  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/

  printf("Client : je termine\n");
  close(ds);
  return 0;
}