#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "http_tools.h"

/**
 * Create a GET request from the given parameters.
 * @param buf Buffer which will contain the request.
 * @param buf_size Buffer size.
 * @param host
 * @param res Resource we want to retrieve.
 * @param port Host port.
 */
void create_GET_request(char* buf, size_t buf_size, const char* host,
    const char* res, const char* port)
{
    snprintf(buf, buf_size,
        "GET %s HTTP/1.1\r\n"
        "Host: %s:%s\r\n"
        "Connection: close\r\n"
        "Accept: text/html\r\n\r\n",
        res, host, port
    );
}

/**
 * Receive et process the GET request in order to retrieve the path to the
 * requested resource.
 *
 * Example :
 *   If the request is :
 *     GET /folder/file.html HTTP/1.0
 *     Host: Localhost:7000
 *   the retrieved resource will be :
 *     /folder/file.html
 *
 * @param scokfd Socket file descriptor
 * @return requested resource
 */
char* recv_res_GET_request(int sockfd) {
    char buf[BUFFER_LEN];
    size_t recv_size = 0;
    size_t buf_offset = 0;
    size_t remaining_buf_size = BUFFER_LEN;
    char *res = NULL;

    /* while there is something to read, and until the resource has been
     * retrieved, process the buffer */
    do {
        int process_status;

        recv_size = read(sockfd, buf + buf_offset, remaining_buf_size - 1);
        if (recv_size == -1) {
            perror("read");
            break;
        }

        buf[buf_offset + recv_size] = '\0';

        res = process_GET_buffer(buf);
        if (res != NULL) {
            break;
        }

        buf_offset += recv_size;
        remaining_buf_size -= recv_size;
    } while (recv_size != 0 && remaining_buf_size > 0);

    return res;
}

/**
 * Retrieve the path to the resource in the GET request by parsing the given
 * buffer.
 *
 * @parram buf Buffer to parse.
 * @return Resource found in the buffer after the parsing.
 */
char* process_GET_buffer(char *buf) {
    char *res = NULL;
    char endline[] = "\r\n";

    if (strstr(buf, endline) != NULL) {
        res = malloc(BUFFER_LEN);

        sscanf(buf, "GET %s", res);
    }

    return res;
}
