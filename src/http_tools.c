#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include "file_tools.h"
#include "http_tools.h"

/**
 * Create a GET request from the given parameters.
 *
 * @param host
 * @param res Resource we want to retrieve.
 * @param port Host port.
 * @return GET request if successful, NULL otherwise
 */
char* create_GET_request(const char* host, const char* res, const char* port) {
    char *buf = malloc(BUFFER_LEN);

    int status = snprintf(buf, BUFFER_LEN,
        "GET %s HTTP/1.1\r\n"
        "Host: %s:%s\r\n"
        "Connection: close\r\n"
        "Accept: text/html\r\n\r\n",
        res, host, port
    );

    if(status < 0) {
        free(buf);
        return NULL;
    }

    return buf;
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
 * Receive a complete request by making multiple recv calls, and each time
 * a recv call returns, prints it. Stops when the string indicating the end of
 * a HTTP request header ("\r\n\r\n") is received.
 *
 * @param sockfd
 * @return 0 on success, -1 otherwise.
*/
int recv_print_request(int sockfd) {
    int recv_size;
    char buf[BUFFER_LEN];

    do {
        recv_size = read(sockfd, buf, BUFFER_LEN - 1);
        if (recv_size == -1) {
            perror("read");
            break;
        }

        if (recv_size != 0) {
            buf[recv_size] = '\0';
            puts(buf);
        }

        if (strstr(buf, "\r\n\r\n") != NULL) {
            break;
        }
    } while(recv_size != 0);

    return recv_size;
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

/**
 * Create a HTTP 200 (OK) response.
 *
 * @param content_type
 * @return HTTP response
 */
char* create_HTTP_200_response(char *content_type) {
    char *buf = malloc(BUFFER_LEN);

    int status = snprintf(buf, BUFFER_LEN,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n\r\n",
        content_type
    );

    puts(buf);

    if(status < 0) {
        free(buf);
        return NULL;
    }

    return buf;
}

/**
 * Send a file (filepath parameter) to a client which handles HTTP requests
 * (probably a browser). Before sending the file, the appropriate HTTP response
 * is sent (200 if the file was found and can be sent, 404 if the file was not
 * found).
 *
 * @param filepath
 * @param fd_dest To whom the file should be sent.
 * @return 0 on success, -1 otherwise.
 */
int sendfile_HTTP_helper(char* filepath, int fd_dest) {
    int filefd, status;
    char *content_type, *response;

    /* retrieve the content_type with the file extension (unsafe, but probably
     * sufficient for this exercise). */
    if (strstr(filepath, ".html") != NULL) {
        content_type = TYPE_HTML;
    }
    else if(strstr(filepath, ".txt") != NULL) {
        content_type = TYPE_PLAIN_TEXT;
    } else {
        content_type = TYPE_OCTET_STREAM;
    }

    // Open requested file
    filefd = open(filepath, O_RDONLY);
    if (filefd == -1) {
        perror("open");

        // file not found -> HTTP 404 response
        if (errno == ENOENT) {
            send_complete(fd_dest, HTTP_404_RESPONSE, strlen(HTTP_404_RESPONSE));
        }

        return -1;
    }

    // HTTP 200 response
    response = create_HTTP_200_response(content_type);
    if (response == NULL) {
        close(filefd);
        return -1;
    }

    // Send the HTTP response to the client
    status = send_complete(fd_dest, response, strlen(response));
    free(response);
    if (status == -1) {
        close(filefd);
        return -1;
    }

    // send requested file
    status = fsendfile_helper(filefd, fd_dest);

    close(filefd);

    return status;
}
