#ifndef SOCKET_TOOLS_H
#define SOCKET_TOOLS_H

#include <netinet/in.h>
#include <string.h>

int sendto_complete(int sockfd, char* msg, int msg_size,
    const struct sockaddr *dest_addr);
int send_complete(int sockfd, char* msg, int msg_size);
int recvfrom_helper(int sockfd, char *buffer, int buffer_size, int *recv_size,
    struct sockaddr *src_addr, socklen_t *addrlen);
int recv_print(int sockfd);

#endif // SOCKET_TOOLS_H
