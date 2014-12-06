#ifndef FILE_TOOLS_H
#define FILE_TOOLS_H

int sendfile_helper(char *filepath, int fd_dest);
int fsendfile_helper(int fd_src, int fd_dest);

#endif // FILE_TOOLS_H
