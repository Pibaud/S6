#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

/* Programme serveur */

int main(int argc, char *argv[]) {

  if (argc != 2){
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */   
  int ds = socket(PF_INET, SOCK_DGRAM, 0);

  if (ds == -1){
    perror("Serveur : pb creation socket :");
    exit(1);
  }
  
  printf("Serveur : creation de la socket réussie \n");
  
  /* Etape 2 : Nommer la socket du seveur */
   struct sockaddr_in adrServ;
   adrServ.sin_family = AF_INET;
   adrServ.sin_port = htons(atoi(argv[1]));
   adrServ.sin_addr.s_addr = INADDR_ANY;

   if(bind(ds, (struct sockaddr*)&adrServ, sizeof(adrServ)) == -1){
     perror("Serveur : erreur bind");
     close(ds);
     exit(1);
   }
 
  /* Etape 4 : recevoir un message du client (voir sujet pour plus de détails)*/

  char msg[100];
  struct sockaddr_in structClient;
  socklen_t addr_len = sizeof(structClient);

  int res = recvfrom(ds, msg, sizeof(msg), 0, (struct sockaddr*)&structClient, &addr_len);

  if(res == -1) {
    perror("Serveur : erreur reception message");
    close(ds);
    exit(1);
  }

  printf("Serveur : le message reçu de %s est : %s\n", inet_ntoa(structClient.sin_addr), msg);
  
  /* Etape 5 : envoyer au client le nombre d'octets reçus (voir sujet pour plus de détails)*/

  if(sendto(ds, &res, sizeof(int), 0, (struct sockaddr*)&structClient, sizeof(structClient)) == -1) {
    perror("Serveur : erreur envoi message");
    close(ds);
    exit(1);
  }
  
  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/

  close(ds);  
  
  printf("Serveur : je termine\n");
  return 0;
}
