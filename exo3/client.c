#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tools.h"

#define REQUEST_MAX_SIZE 512

/*
Exercice 3
Ecrire un client qui reçoit en argument :
• Le nom du serveur auquel on va chercher à ce connecter
• Le numéro de port de ce serveur
• Un nom de fichier
Ce client doit se connecter à un serveur web standard (par exemple intra.esiee.fr) pour
lui envoyer une requête http demandant l’affichage du nom de fichier donné en argument (par
exemple index.html). Pour construire la requête, vous utiliserez les informations obtenues par
le serveur de l’exercice 1.
Après avoir envoyé sa requête, votre client doit attendre et afficher les données
envoyées par le serveur.
Vous vous limiterez au format de pages html pour ce TP
*/

void create_GET_request(char* out, const char* host, const char* res,
    const char* port)
{
    snprintf(out, REQUEST_MAX_SIZE,
        "GET %s HTTP/1.1\n"
        "Host: %s:%s\n"
        "Accept: text/html\n",
        res, host, port
    );
}

/**
* Client entry point, the following arguments are needed :
*    - server hostname / server IP
*    - server port
*    - filename (resource we want to retrieve on the server)
*/
int main(int argc, const char* argv[]) {
    int sockfd, status;
    struct hostent *he;
    struct sockaddr_in dest_addr;
    char request[REQUEST_MAX_SIZE];

    if (argc < 4) {
        printf("Missing arguments\nUsage : %s hostname port filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Retrieve server information
    he = gethostbyname(argv[1]);
    if (he == NULL) {
        perror("client - gethostbyname");
        return EXIT_FAILURE;
    }

    // Create socket (IPv4, connected, TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("client - socket init");
        return EXIT_FAILURE;
    }

    // Fill dest_addr
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(atoi(argv[2]));
    dest_addr.sin_addr = *((struct in_addr*) he->h_addr);
    memset(dest_addr.sin_zero, 0, sizeof(dest_addr.sin_zero));

    // Connect the socket
    status = connect(sockfd, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
    if (status == -1) {
        perror("client - connect");
        return EXIT_FAILURE;
    }

    // Send request
    create_GET_request(request, argv[1], argv[3], argv[2]);
    status = send_complete(sockfd, request, strlen(request));
    if (status == -1) {
        printf("client - request could not completely be sent.\n");
        return EXIT_FAILURE;
    }

    // TODO recv complete response

    close(sockfd);

    return EXIT_SUCCESS;
}
