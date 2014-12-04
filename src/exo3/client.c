#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_tools.h"
#include "http_tools.h"

#define REQUEST_LEN 512

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
    char request[REQUEST_LEN];

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
    create_GET_request(request, REQUEST_LEN, argv[1], argv[3], argv[2]);
    status = send_complete(sockfd, request, strlen(request));
    if (status == -1) {
        printf("client - request could not completely be sent.\n");
        return EXIT_FAILURE;
    }

    puts(request);

    status = recv_print(sockfd);
    if (status == -1) {
        printf("client - error during recv.\n");
        return EXIT_FAILURE;
    }

    close(sockfd);

    return EXIT_SUCCESS;
}
