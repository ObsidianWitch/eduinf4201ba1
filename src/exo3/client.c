#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_tools.h"
#include "http_tools.h"

/**
 * Client entry point, the following arguments are needed :
 *    - server hostname / server IP
 *    - server port
 *    - filename (resource we want to retrieve on the server)
 */
int main(int argc, const char* argv[]) {
    int sockfd, status;
    char *request;

    if (argc < 4) {
        printf("Missing arguments\nUsage : %s hostname port filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    sockfd = init_stream_client_socket(argv[1], atoi(argv[2]));

    // Send request
    request = create_GET_request(argv[1], argv[3], argv[2]);
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
