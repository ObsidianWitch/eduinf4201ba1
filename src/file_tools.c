#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "file_tools.h"

/**
* Send a file (filepath parameter) to a destination (fd_dest paramter).
*
* @param filepath
* @param fd_dest To whom the file should be sent.
* @return 0 on success, -1 otherwise.
*/
int sendfile_helper(char *filepath, int fd_dest) {
    int filefd, status;

    filefd = open(filepath, O_RDONLY);
    if (filefd == -1) {
        perror("open");
        return -1;
    }

    status = fsendfile_helper(filefd, fd_dest);

    close(filefd);

    return status;
}

/**
* Send a file (fd_src) to a destination corresponding to fd_dest.
*
* @param fd_src Source file descriptor
* @param fd_dest Destination file descriptor
* @return 0 on success, -1 otherwise.
*/
int fsendfile_helper(int fd_src, int fd_dest) {
    int status;
    struct stat st;

    status = fstat(fd_src, &st); // Retrieve file size
    if (status == -1) {
        perror("fstat");
        return -1;
    }

    status = sendfile(fd_dest, fd_src, NULL, st.st_size);
    if (status == -1) {
        perror("sendfile");
    }

    return status;
}
