#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

/**
* Send a complete message with the connected socket specified in parameter
* (sockfd file descriptor).
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
