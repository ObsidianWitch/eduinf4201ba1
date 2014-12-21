#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "exo1/msg_tools.h"
#include "socket_tools.h"

#define BUFFER_SIZE 256

/**
 * Client entry point, the following arguments are needed :
 *    - server hostname / server IP
 *    - server port
 *    - message
 */
int main(int argc, const char* argv[]) {
    int sockfd, status, msg_size, recv_size;
    struct hostent *he;
    struct sockaddr_in dest_addr;
    char* msg, *buffer;

    if (argc < 4) {
        printf("Missing arguments\nUsage : %s hostname port message\n", argv[0]);
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
    dest_addr.sin_addr = *((struct in_addr*) he->h_addr_list[0]);
    memset(dest_addr.sin_zero, 0, sizeof(dest_addr.sin_zero));

    // Create and send message to server
    msg = create_msg(argv[3]);
    msg_size = strlen(msg) + 1;
    status = sendto_complete(sockfd, msg, msg_size, (struct sockaddr*) &dest_addr);
    if (status == -1) {
        printf("client - the message could not be completely sent to the server.");
    }

    // receive message from the server
    buffer = malloc(BUFFER_SIZE);
    recvfrom_helper(sockfd, buffer, msg_size, &recv_size , NULL, NULL);
    printf("client - received %d bytes : %s\n", recv_size, buffer);

    free(msg);
    free(buffer);
    close(sockfd);

    return EXIT_SUCCESS;
}
