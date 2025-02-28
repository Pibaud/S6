#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

/* Programme client */

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

   if (argc != 3)
   {
      printf("utilisation : %s ip_serveur port_serveur\n", argv[0]);
      exit(1);
   }

   /* Etape 1 : créer une socket */
   int ds = socket(PF_INET, SOCK_STREAM, 0);

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

   if (connect(ds, (struct sockaddr *)&adrServ, sizeof(adrServ)) == -1)
   {
      perror("Client : erreur de connection au serveur");
      close(ds);
      exit(1);
   }

   printf("Client : connexion réussie\n");

   /* Etape 4 : envoyer un message au serveur  (voir sujet pour plus de détails)*/
   char msg[100];
   printf("Client : saisir le message à envoyer au serveur\n");
   fgets(msg, 100, stdin);

   int msgLength = strlen(msg);
   if (sendTCP(ds, &msgLength, sizeof(int)) <= 0)
   {
      perror("Client : erreur d'envoi");
      close(ds);
      exit(1);
   }

   printf("Client : taille du message envoyée : %d\n", msgLength);

   if (sendTCP(ds, msg, strlen(msg)) <= 0)
   {
      perror("Client : erreur d'envoi");
      close(ds);
      exit(1);
   }

   printf("Client : message envoyé\n");

   /* Etape 5 : recevoir un message du serveur (voir sujet pour plus de détails)*/

   int retour;
   int res = recvTCP(ds, &retour, sizeof(int));
   if (res <= 0)
   {
      perror("Client : erreur de reception");
      close(ds);
      exit(1);
   }

   /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
   printf("Client : nombre d'octets reçus : %d\n", retour);

   close(ds);
   printf("Client : je termine\n");
   return 0;
}
