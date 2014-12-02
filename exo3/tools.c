#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

/**
 * Send a complete message with the connected socket specified in parameter
 * (sockfd file descriptor).
 *
 * @param sockfd sending connected socket
 * @param msg message to send
 * @param msg_size size of the message to send
 * @param addrlen size of dest_addr
 * @return Returns 0 on success, and -1 if the message could not completely be
 * sent.
 */
int send_complete(int sockfd, char* msg, int msg_size) {
    int sent_size = 0;

    while (sent_size < msg_size) {
        int remaining_size = msg_size - sent_size;
        int temp_size = send(sockfd, msg + sent_size, remaining_size, 0);
        if (temp_size == -1) {
            perror("send");
            break;
        }

        sent_size += temp_size;
    }

    return (sent_size != msg_size) ? -1 : 0;
}

/**
 * Receive a complete message by making multiple recv calls, and each time
 * a recv call returns, prints it.
 *
 * @param sockfd
 * @param buf buffer
 * @param len size of the buffer
 * @return 0 on success, -1 otherwise.
 */
int recv_print(int sockfd, char *buf, size_t len) {
    int recv_size;

    do {
        recv_size = read(sockfd, buf, len - 1);
        if (recv_size == -1) {
            perror("recv");
            break;
        }

        if (recv_size != 0) {
            buf[recv_size] = '\0';
            puts(buf);
        }
    } while(recv_size != 0);

    return recv_size;
}
