#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_tools.h"
#include "http_tools.h"

/**
 * Server entry point, the following arguments are required :
 *     - port on which the clients will connect
 */
int main(int argc, const char* argv[]) {
    int sockfd, clientfd;

    if (argc < 2) {
        printf("Missing arguments\nUsage : %s port\n", argv[0]);
        return EXIT_FAILURE;
    }

    sockfd = init_stream_server_socket(atoi(argv[1]));

    // Accept one connection request from a client
    clientfd = accept(sockfd, NULL, NULL);
    if (clientfd == -1) {
        perror("server - accept");
        return EXIT_FAILURE;
    }

    recv_print_request(clientfd);

    close(clientfd);
    close(sockfd);

    return EXIT_SUCCESS;
}
