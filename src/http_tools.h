#ifndef HTTP_TOOLS
#define HTTP_TOOLS

void create_GET_request(char* out, size_t out_size, const char* host,
    const char* res, const char* port);
void parse_GET_request(char* in, char* host, char *res, int *port);

#endif // HTTP_TOOLS
