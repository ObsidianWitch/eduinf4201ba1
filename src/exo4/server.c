#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
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

#define BUFFER_LEN 1024
#define DEFAULT_PAGE "index.html"

/*
TODO
Exercice 4
Ecrire un petit serveur web qui réponde aux requêtes du client écrit précédemment. Ce
serveur stockera dans un fichier local (log_file) toutes les requêtes servies ou non. Pour
chaque requête ce fichier contiendra l’adresse de la machine du client, la date et le fichier
demandé.
Ce serveur recevra 2 numéros de port en argument. Il répondra normalement aux
requêtes http sur le premier port, et retournera son fichier de log si un client s’y connecte par
le second port.
*/

int logfd; // Log file descriptor

int init_stream_server_socket(int port);
int handle_GET_request(int clientfd, struct in_addr client_addr);
int log_line(struct in_addr client_addr, char *res);

/**
 * Server entry point, the following arguments are required :
 *     - 1st port on which the clients will connect (GET request)
 *     - 2nd port on which the clients will connect (retrieve logs)
 */
int main(int argc, const char* argv[]) {
    int sockfd_http, sockfd_log, clientfd;
    char buffer[BUFFER_LEN];
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;

    if (argc < 3) {
        printf("Missing arguments\nUsage : %s port_http port_log\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open log file
    logfd = open("log.txt", O_CREAT|O_RDWR|O_APPEND);
    if (logfd == -1) {
        perror("open log");
        return EXIT_FAILURE;
    }

    // Open sockets
    sockfd_http = init_stream_server_socket(atoi(argv[1]));
    sockfd_log = init_stream_server_socket(atoi(argv[2]));


    /*// TODO accept multiple client from multiple ports -> fork, select ?
    // Handle requests from clients connecting to the first port
    // Accept one connection request from a client
    client_addrlen = sizeof(client_addr);
    clientfd = accept(sockfd_http, (struct sockaddr *) &client_addr, &client_addrlen);
    if (clientfd == -1) {
        perror("server (http) - accept");
        return EXIT_FAILURE;
    }

    handle_GET_request(clientfd, client_addr.sin_addr); // TODO receive multiple requests*/

    // TODO accept multiple client from multiple ports -> fork, select ?
    // Send log file to clients connecting to the second port
    // Accept one connection request from a client
    clientfd = accept(sockfd_log, NULL, NULL);
    if (clientfd == -1) {
        perror("server (log) - accept");
        return EXIT_FAILURE;
    }

    fsendfile_helper(logfd, clientfd);

    close(sockfd_http);
    close(sockfd_log);
    close(clientfd);
    close(logfd);

    return EXIT_SUCCESS;
}

/**
 * Insert a line in the log file in the following format :
 *     client_addr - date - resource
 *
 * @param client_addr
 * @param res
 * @return Returns 0 on success, or -1 if the line could not be added to the log.
 */
int log_line(struct in_addr client_addr, char *res) {
    char datestr[200];
    char *client_addr_str = inet_ntoa(client_addr);
    time_t t = time(NULL);
    struct tm *tmp;

    // date
    tmp = localtime(&t);
    if (tmp == NULL) {
        perror("localtime");
        return -1;
    }

    if (strftime(datestr, 200, "%T", tmp) == 0) {
        puts("strftime - could not write the date into datestr");
        return -1;
    }

    int str_size = strlen(client_addr_str) + strlen(datestr) + strlen(res) + 8;
    char *str = malloc(str_size);

    snprintf(str, str_size,
        "%s - %s - %s\n",
        client_addr_str, datestr, res
    );

    write(logfd, str, str_size);

    free(str);

    return 0;
}

/*
 * Handle a GET request from a client.
 *
 * @param clientfd File descriptor from the client sending the GET request.
 * @param client_addr client IP address (log)
 * @return 0 on success, -1 otherwise;
 */
int handle_GET_request(int clientfd, struct in_addr client_addr) {
    int filefd, status;
    char *path, *res;

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

    status = sendfile_helper(path, clientfd);

    free(res);

    return status;
}

/**
 * Initialize a stream socket used as a server.
 * Exits the application if one of the steps to create and initialize the socket
 * fails.
 *
 * @param port on which the socket will be bound
 * @return File descriptor of the created socket.
 */
int init_stream_server_socket(int port) {
    int sockfd, status;
    struct sockaddr_in serv_addr;

    // Create socket (IPv4, connected, TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("server - socket init");
        exit(EXIT_FAILURE);
    }

    // Fill serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    memset(serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    // Bind socket to all local interfaces on the port specified in argument
    status = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (status == -1) {
        perror("server - bind");
        exit(EXIT_FAILURE);
    }

    // Passive (listening) socket
    status = listen(sockfd, 1);
    if (status == -1) {
        perror("server - listen");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}
