#include <netinet/in.h>
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 2){
        printf("utilisation : %s port_serveur\n", argv[0]);
        exit(1);
    }

    int ds = socket(PF_INET, SOCK_DGRAM, 0);
    if (ds == -1){
        perror("Serveur : pb creation socket :");
        exit(1);
    }
    printf("Serveur : création de la socket réussie \n");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[1]));
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t tailleStructServ = sizeof(serverAddr);
    if (bind(ds, (struct sockaddr*)&serverAddr, tailleStructServ) < 0) {
        perror("Serveur : erreur lors du bind :");
        close(ds);
        exit(1);
    }
    printf("Serveur : bind réussi\n");

    while (1) {
        char msgCli[100] = {0};
        struct sockaddr_in clientAddr;
        socklen_t tailleStructClient = sizeof(clientAddr);

        int rcv = recvfrom(ds, msgCli, sizeof(msgCli), 0, (struct sockaddr*)&clientAddr, &tailleStructClient);
        if (rcv < 0) {
            perror("Serveur : erreur réception message :");
            continue;
        }
        msgCli[rcv] = '\0'; // Assurez-vous que la chaîne est terminée
        printf("Serveur : reçu '%s' de %s:%d\n", msgCli, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        int resp = strlen(msgCli);
        if (sendto(ds, &resp, sizeof(int), 0, (struct sockaddr*)&clientAddr, tailleStructClient) < 0) {
            perror("Serveur : erreur envoi réponse :");
        } else {
            printf("Serveur : réponse envoyée : %d octets\n", resp);
        }
    }

    close(ds);
    printf("Serveur : je termine\n");
    return 0;
}