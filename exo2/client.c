#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tools.h"

/**
 * Client entry point, the following arguments are needed :
 *    - server hostname / server IP
 *    - server port
 */
int main(int argc, const char* argv[]) {
    int sockfd, status;
    struct hostent *he;
    struct sockaddr_in dest_addr;
    char c;

    if (argc < 3) {
        printf("Missing arguments\nUsage : %s hostname port\n", argv[0]);
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

    do {
        int sent_size;

        c = getchar();
        sent_size = send(sockfd, &c, 1, 0);
        if (sent_size == -1) {
            perror("client - send");
        }
    } while (c != EOF);

    close(sockfd);

    return EXIT_SUCCESS;
}
