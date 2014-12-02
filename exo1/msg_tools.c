#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msg_tools.h"

/**
 * Create a message with the following format :
 * out = "pid: in"
 * @param in Part of the message to create.
 * @return created message
 */
char* create_msg(const char* in) {
    char* out;

    int pid_length = snprintf(NULL, 0, "%d", getpid());
    int msg_size = sizeof(char) * (pid_length + strlen(in) + 3);
    out = malloc(msg_size);
    snprintf(out, msg_size, "%d: %s", getpid(), in);

    return out;
}

char* extract_msg(char* in, size_t in_len) {
    int pid;
    char *out = malloc(sizeof(char) * in_len);

    sscanf(in, "%d: %s", &pid, out);

    return out;
}
