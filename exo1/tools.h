#ifndef TOOLS_H
#define TOOLS_H

int sendto_complete(int sockfd, char* msg, int msg_size,
    const struct sockaddr *dest_addr);
void create_msg(char* out, char* in);

#endif // TOOLS_H
