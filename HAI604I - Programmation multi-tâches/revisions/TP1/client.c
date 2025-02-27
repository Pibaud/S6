#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

/* Programme client */

int main(int argc, char *argv[])
{

   if (argc != 3)
   {
      printf("utilisation : %s ip_serveur port_serveur\n", argv[0]);
      exit(1);
   }

   /* Etape 1 : créer une socket */
   int ds = socket(PF_INET, SOCK_DGRAM, 0);

   if (ds == -1)
   {
      perror("Client : pb creation socket :");
      exit(1); // je choisis ici d'arrêter le programme car le reste
               // dépendent de la réussite de la création de la socket.
   }

   /* J'ajoute des traces pour comprendre l'exécution et savoir
      localiser des éventuelles erreurs */
   printf("Client : creation de la socket réussie \n");

   // Je peux tester l'exécution de cette étape avant de passer à la
   // suite. Faire de même pour la suite : n'attendez pas de tout faire
   // avant de tester.

   /* Etape 2 : Nommer la socket du client */

   /* Etape 3 : Désigner la socket du serveur */
   struct sockaddr_in adrServ;
   adrServ.sin_family = AF_INET;
   adrServ.sin_port = htons(atoi(argv[2]));
   adrServ.sin_addr.s_addr = inet_addr(argv[1]);

   /* Etape 4 : envoyer un message au serveur  (voir sujet pour plus de détails)*/
   char msg[100];
   printf("Client : saisir le message à envoyer au serveur\n");
   fgets(msg, 100, stdin);

   if (sendto(ds, msg, strlen(msg), 0, (struct sockaddr *)&adrServ, sizeof(adrServ)) == -1)
   {
      perror("Client : erreur d'envoi");
      close(ds);
      exit(1);
   }

   /* Etape 5 : recevoir un message du serveur (voir sujet pour plus de détails)*/

   int retour;
   struct sockaddr_in adrServRecv;
   socklen_t addrLen = sizeof(adrServRecv);
   if (recvfrom(ds, &retour, sizeof(int), 0, (struct sockaddr *)&adrServRecv, &addrLen) == -1)
   {
      perror("Client : erreur de reception");
      close(ds);
      exit(1);
   }

   /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
   printf("Client : nombre d'octets reçus de %s : %d\n", inet_ntoa(adrServRecv.sin_addr), retour - 1);

   close(ds);
   printf("Client : je termine\n");
   return 0;
}
