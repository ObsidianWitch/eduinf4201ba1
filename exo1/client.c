#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
TODO Le client envoie vers le serveur : son PID + la chaine qui lui a été passée
TODO gethostbyname
*/

// TODO test sans le serveur (en écoutant les packets reçu en local)

/**
 * Client entry point, the following arguments are needed :
 *    - server hostname / server IP
 *    - server port
 *    - message
 */
int main(int argc, const char* argv[]) {
    int sockfd;
    struct hostent *he;
    struct sockaddr_in dest_addr;
    char* msg;

    if (argc < 4) {
        puts("Missing arguments.");
        printf("Usage : %s hostname port message\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Retrieve server information
    he = gethostbyname(argv[1]);
    if (he == NULL) {
        perror("client - gethostbyname");
        return EXIT_FAILURE;
    }

    // Create socket (IPv4, connectionless, UDP)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("client - socket init");
        return EXIT_FAILURE;
    }

    // fill dest_addr
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(atoi(argv[2]));
    dest_addr.sin_addr = *((struct in_addr*) he->h_addr);
    memset(dest_addr.sin_zero, 0, sizeof(dest_addr.sin_zero));

    // Create and send message to server
    create_msg(msg, argv[3]);
    sendto_complete(sockfd, msg, strlen(msg), (struct sockaddr*) &dest_addr);

    // TODO receive response

    // TODO close/free

    //free(he);
    //free(msg);
    close(sockfd);

    return EXIT_SUCCESS;
}
