#ifndef TOOLS_H
#define TOOLS_H

#include <netinet/in.h>

int sendto_complete(int sockfd, char* msg, int msg_size,
    const struct sockaddr *dest_addr);
int recvfrom_helper(int sockfd, char *buffer, int buffer_size, int *recv_size,
    struct sockaddr *src_addr, socklen_t *addrlen);
char* create_msg(const char* in);
char* extract_msg(char* in, size_t in_len);

#endif // TOOLS_H
