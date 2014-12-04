#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_tools.h"
#include "http_tools.h"

#define BUFFER_LEN 1024

/*
Exercice 4
Ecrire un petit serveur web qui réponde aux requêtes du client écrit précédemment. Ce
serveur stockera dans un fichier local (log_file) toutes les requêtes servies ou non. Pour
chaque requête ce fichier contiendra l’adresse de la machine du client, la date et le fichier
demandé.
Ce serveur recevra 2 numéros de port en argument. Il répondra normalement aux
requêtes http sur le premier port, et retournera son fichier de log si un client s’y connecte par
le second port.
*/

//#define BUFFER_LEN 1024

/**
 * Server entry point, the following arguments are required :
 *     - 1st port on which the clients will connect (GET request)
 *     - 2nd port on which the clients will connect (retrieve logs)
 */
int main(int argc, const char* argv[]) {
    int sockfd_http, sockfd_log, clientfd;
    char buffer[BUFFER_LEN];

    if (argc < 3) {
        printf("Missing arguments\nUsage : %s port_http port_log\n", argv[0]);
        return EXIT_FAILURE;
    }

    sockfd_http = init_stream_server_socket(atoi(argv[1]));
    sockfd_log = init_stream_server_socket(atoi(argv[2]));

    // TODO accept multiple client from multiple ports -> fork

    // Accept one connection request from a client
    clientfd = accept(sockfd_http, NULL, NULL);
    if (clientfd == -1) {
        perror("server - accept");
        return EXIT_FAILURE;
    }

    // TODO receive multiple requests

    char *res = recv_res_GET_request(clientfd);
    if (res != NULL) {
        puts(res); // TODO handle request : HTTP header for sending file + file

        // TODO if file could not be found, send corresponding HTTP header
    }
    free(res);

    close(sockfd_http);
    close(sockfd_log);
    close(clientfd);

    return EXIT_SUCCESS;
}

/**
 * Initialize a stream socket used as a server.
 * Exits the application if one of the steps to create and initialize the socket
 * fails.
 *
 * @param port on which the socket will be bound
 * @return File descriptor of the created socket.
 */
int init_stream_server_socket(int port) {
    int sockfd, status;
    struct sockaddr_in serv_addr;

    // Create socket (IPv4, connected, TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("server - socket init");
        exit(EXIT_FAILURE);
    }

    // Fill serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    // Bind socket to all local interfaces on the port specified in argument
    status = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (status == -1) {
        perror("server - bind");
        exit(EXIT_FAILURE);
    }

    // Passive (listening) socket
    status = listen(sockfd, 1);
    if (status == -1) {
        perror("server - listen");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}
