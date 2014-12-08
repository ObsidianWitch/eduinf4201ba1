#include <sys/sendfile.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "socket_tools.h"
#include "http_tools.h"
#include "file_tools.h"

#define DEFAULT_PAGE "index.html"

/*
TODO
Exercice 4
Ecrire un petit serveur web qui
- répond aux requêtes http sur le premier port
- retourne son fichier de log sur le second port
*/

int handle_GET_request(int clientfd, struct in_addr client_addr);
int log_line(struct in_addr client_addr, char *res);

/**
 * Server entry point, the following arguments are required :
 *     - 1st port on which the clients will connect (GET request)
 *     - 2nd port on which the clients will connect (retrieve logs)
 */
int main(int argc, const char* argv[]) {
    int sockfd_http, sockfd_log, status;
    fd_set master, readfds;

    if (argc < 3) {
        printf("Missing arguments\nUsage : %s port_http port_log\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open sockets
    sockfd_http = init_stream_server_socket(atoi(argv[1]));
    sockfd_log = init_stream_server_socket(atoi(argv[2]));

    // Clear all entries from the sets, and add the sockets fd to master.
    FD_ZERO(&readfds);
    FD_ZERO(&master);
    FD_SET(sockfd_http, &master);
    FD_SET(sockfd_log, &master);

    // Main loop
    while(1) {
        /* Copy master into readfds. It must be done before each call to select,
         * since the sets are modified when select returns. */
        readfds = master;

        status = select(sockfd_log + 1, &readfds, NULL, NULL, NULL);
        if (status == -1) {
            perror("select");
        }
        else if (status != 0) {
            if (FD_ISSET(sockfd_http, &readfds)) {
                struct sockaddr_in client_addr;
                socklen_t client_addrlen = sizeof(client_addr);

                int clientfd = accept(sockfd_http, (struct sockaddr *) &client_addr,
                    &client_addrlen);
                if (clientfd == -1) {
                    perror("server (http) - accept");
                }

                handle_GET_request(clientfd, client_addr.sin_addr);

                close(clientfd);
            }

            if (FD_ISSET(sockfd_log, &readfds)) {
                int clientfd = accept(sockfd_log, NULL, NULL);
                if (clientfd == -1) {
                    perror("server (log) - accept");
                }

                // receive request from the client but don't use it
                recv_print_request(clientfd);

                sendfile_HTTP_helper("log.txt", clientfd);

                close(clientfd);
            }
        }
    }

    close(sockfd_http);
    close(sockfd_log);

    return EXIT_SUCCESS;
}

/**
 * Insert a line in the log file with the following format :
 *     client_addr - date - resource
 *
 * @param client_addr
 * @param res
 * @return Returns 0 on success, or -1 if the line could not be added to the log.
 */
int log_line(struct in_addr client_addr, char *res) {
    int logfd;
    char datestr[200];
    char *client_addr_str = inet_ntoa(client_addr);

    // Retrieve date
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);
    if (tmp == NULL) {
        perror("localtime");
        return -1;
    }

    if (strftime(datestr, 200, "%T", tmp) == 0) {
        puts("strftime - could not write the date into datestr");
        return -1;
    }

    // Create string which will be inserted into the log file
    int str_size = strlen(client_addr_str) + strlen(datestr) + strlen(res) + 8;
    char *str = malloc(str_size);

    snprintf(str, str_size,
        "%s - %s - %s\n",
        client_addr_str, datestr, res
    );

    // Open log file
    logfd = open("log.txt", O_CREAT|O_WRONLY|O_APPEND, 0664);
    if (logfd == -1) {
        perror("open log");
        free(str);
        return -1;
    }

    write(logfd, str, strlen(str));

    free(str);
    close(logfd);

    return 0;
}

/*
 * Handle a GET request from a client.
 *
 * @param clientfd File descriptor from the client sending the GET request.
 * @param client_addr client IP address (log information)
 * @return 0 on success, -1 otherwise.
 */
int handle_GET_request(int clientfd, struct in_addr client_addr) {
    int status;
    char *path, *res;

    // Retrieve resource
    res = recv_res_GET_request(clientfd);
    if (res == NULL) {
        return -1;
    }

    log_line(client_addr, res);

    if (strcmp(res, "/") == 0) {
        path = DEFAULT_PAGE;
    }
    else {
        path = res + 1; // file path without the leading "/"
    }

    status = sendfile_HTTP_helper(path, clientfd);

    free(res);

    return status;
}
