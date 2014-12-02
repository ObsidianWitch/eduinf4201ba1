#include "tools.h"

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
