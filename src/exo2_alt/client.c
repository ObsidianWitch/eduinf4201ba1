#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_tools.h"

/**
 * Client entry point, the following arguments are needed :
 *    - server hostname / server IP
 *    - server port
 */
int main(int argc, const char* argv[]) {
    int sockfd;
    char c;

    if (argc < 3) {
        printf("Missing arguments\nUsage : %s hostname port\n", argv[0]);
        return EXIT_FAILURE;
    }

    sockfd = init_stream_client_socket_alt(argv[1], argv[2]);

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
