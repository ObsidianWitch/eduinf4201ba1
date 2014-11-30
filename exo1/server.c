#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tools.h"

#define BUFFER_SIZE 256

/**
* Server entry point, the following arguments are required :
*     - port on which the clients will connect
*/
int main(int argc, const char* argv[]) {
    int sockfd, status;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    if (argc < 2) {
        printf("Missing arguments\nUsage : %s port\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Create socket (IPv4, connectionless, UDP)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("server - socket init");
        return EXIT_FAILURE;
    }

    // fill serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    // bind socket to all local interfaces on the port specified in argument
    status = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (status == -1) {
        perror("server - bind");
        return EXIT_FAILURE;
    }

    // listening loop
    while(1) {
        struct sockaddr_in client_addr;
        int status, recv_size;

        // receive message from a client
        int client_addr_size = sizeof(client_addr);
        status = recvfrom_helper(sockfd, buffer, BUFFER_SIZE, &recv_size,
        (struct sockaddr *) &client_addr, &client_addr_size);
        if (status == -1) {
            continue;
        }

        printf("server - received %d bytes : %s\n", recv_size, buffer);

        // send to the client the message we previously received
        status = sendto_complete(sockfd, buffer, recv_size,
            (struct sockaddr *) &client_addr);

        if (status == -1) {
            printf("server - the message could not be completely sent to %d",
            inet_ntoa(client_addr.sin_addr));
        }
    }

    close(sockfd);

    return EXIT_SUCCESS;
}
