#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

/**
 * Send a complete message with the connectionless socket specified in parameter
 * (sockfd file descriptor).
 * @param sockfd sending connectionless socket
 * @param msg message to send
 * @param msg_size size of the message to send
 * @param dest_addr address of the target
 * @return Returns 0 on success, and -1 if the message could not completely be
 * sent.
 */
int sendto_complete(int sockfd, char* msg, int msg_size,
    const struct sockaddr *dest_addr)
{
    int sent_size = 0;

    while (sent_size < msg_size) {
        int remaining_size = msg_size - sent_size;
        int temp_size = sendto(sockfd, msg + sent_size, remaining_size, 0,
            dest_addr, sizeof(*dest_addr));
        if (temp_size == -1) {
            perror("sendto");
            break;
        }

        sent_size += temp_size;
    }

    return (sent_size != msg_size) ? -1 : 0;
}

/**
 * Receive a message from a socket and adds a trailing '\0' to it.
 * @return Returns 0 on success, -1 otherwise.
 */
int recvfrom_helper(int sockfd, char *buffer, int buffer_size, int *recv_size,
    struct sockaddr *src_addr, socklen_t *addrlen)
{
    *recv_size = recvfrom(sockfd, buffer, buffer_size, 0, src_addr, addrlen);
    if (*recv_size == -1) {
        perror("recvfrom");
        return -1;
    }

    // Make sure the received message ends with '\0'.
    if (buffer[*recv_size - 1] != '\0') {
        if (*recv_size == buffer_size) {
            buffer[*recv_size - 1] = '\0';
        }
        else {
            buffer[*recv_size] = '\0';
        }
    }

    return 0;
}

/**
 * Create a message with the following format :
 * out = "pid: in"
 * @param in Part of the message to create.
 * @return created message
 */
char* create_msg(const char* in) {
    char* out;

    int pid_length = snprintf(NULL, 0, "%d", getpid());
    int msg_size = sizeof(char) * (pid_length + strlen(in) + 3);
    out = malloc(msg_size);
    snprintf(out, msg_size, "%d: %s", getpid(), in);

    return out;
}
