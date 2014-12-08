#ifndef HTTP_TOOLS
#define HTTP_TOOLS

#define BUFFER_LEN 1024

#define HTTP_404_RESPONSE "HTTP/1.1 404 Not Found\r\n"\
                          "Content-Type: text/html\r\n\r\n"\
                          "<!DOCTYPE html>\r\n"\
                          "<html><body>\r\n"\
                          "<h1>Not Found</h1>\r\n"\
                          "</body></html>"\

#define TYPE_HTML "text/html"
#define TYPE_PLAIN_TEXT "text/plain"
#define TYPE_OCTET_STREAM "application/octet-stream"

char* create_GET_request(const char* host, const char* res, const char* port);
char* recv_res_GET_request(int sockfd);
int recv_print_request(int sockfd);
char* process_GET_buffer(char *buf);
int sendfile_HTTP_helper(char* filepath, int fd_dest);

#endif // HTTP_TOOLS
