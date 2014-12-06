#ifndef HTTP_TOOLS
#define HTTP_TOOLS

#define BUFFER_LEN 1024
#define HTTP_404_RESPONSE "HTTP/1.1 404 \r\n\r\n"

void create_GET_request(char* buf, size_t buf_size, const char* host,
    const char* res, const char* port);
char* recv_res_GET_request(int sockfd);
char* process_GET_buffer(char *buf);

#endif // HTTP_TOOLS
