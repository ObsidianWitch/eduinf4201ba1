#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_LEN 1024

/**
* Server entry point, the following arguments are required :
*     - port on which the clients will connect
*/
int main(int argc, const char* argv[]) {
    int sockfd, clientfd, status;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_LEN];

    if (argc < 2) {
        printf("Missing arguments\nUsage : %s port\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Create socket (IPv4, connected, TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("server - socket init");
        return EXIT_FAILURE;
    }

    // Fill serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    // Bind socket to all local interfaces on the port specified in argument
    status = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (status == -1) {
        perror("server - bind");
        return EXIT_FAILURE;
    }

    // Passive (listening) socket
    status = listen(sockfd, 1);
    if (status == -1) {
        perror("server - listen");
        return EXIT_FAILURE;
    }

    // Accept one connection request from a client
    clientfd = accept(sockfd, NULL, NULL);
    if (clientfd == -1) {
        perror("server - accept");
        return EXIT_FAILURE;
    }

    recv_print(clientfd, buffer, BUFFER_LEN);

    close(sockfd);
    close(clientfd);

    return EXIT_SUCCESS;
}
