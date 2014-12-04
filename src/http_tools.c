#include <stdio.h>
#include "http_tools.h"

void create_GET_request(char* out, size_t out_size, const char* host,
    const char* res, const char* port)
{
    snprintf(out, out_size,
        "GET %s HTTP/1.1\r\n"
        "Host: %s:%s\r\n"
        "Connection: close\r\n"
        "Accept: text/html\r\n\r\n",
        res, host, port
    );
}

/**
 * Receive a GET request. A GET request is completely received if there is
 * nothing else to receive (size received 0) or if \r\n\r\n is detected.
 */
void recv_GET_request(int sockfd, char* buf, size_t buf_size) {
    // TODO strstr to find a substring
}

void parse_GET_request(char* in, char* host, char *res, int *port) {
    // TODO fill host, res and port with sscanf
}
