#ifndef TOOLS_H
#define TOOLS_H

int send_complete(int sockfd, char* msg, int msg_size);
int recv_print(int sockfd, char *buf, size_t len);

#endif // TOOLS_H
