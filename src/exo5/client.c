#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_tools.h"
#include "http_tools.h"

/**
 * Client entry point, the following arguments are needed :
 *    - full URL (e.g http://www.google.fr/)
 */
int main(int argc, const char* argv[]) {
    int sockfd, status;
    char *request;

    if (argc < 2) {
        printf(
            "Missing arguments\n"
            "Usage : %s full_URL\n"
            "Example: %s http://www.google.fr/",
            argv[0], argv[0]
        );
        return EXIT_FAILURE;
    }

    sockfd = init_stream_client_socket(ESIEE_PROXY_IP, atoi(ESIEE_PROXY_PORT));

    // Send request
    request = create_GET_request(ESIEE_PROXY_IP, argv[1], ESIEE_PROXY_PORT);
    if (request == NULL) {
        printf("client - could not create the GET request.");
        return EXIT_FAILURE;
    }
    puts(request);

    status = send_complete(sockfd, request, strlen(request));
    free(request);
    if (status == -1) {
        printf("client - request could not completely be sent.\n");
        return EXIT_FAILURE;
    }

    status = recv_print(sockfd);
    if (status == -1) {
        printf("client - error during recv.\n");
        return EXIT_FAILURE;
    }

    close(sockfd);

    return EXIT_SUCCESS;
}
