#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "func.h"

void print_file_infor(const struct stat *info, const char *filename) {
    printf("File name: %s\n", filename);
    printf("File size: %ld bytes\n", info->st_size);
    printf("Last modified: %s", ctime(&info->st_mtime));

    if (S_ISREG(info->st_mode))      printf("File type: Regular file\n");
    else if (S_ISDIR(info->st_mode)) printf("File type: Directory\n");
    else if (S_ISLNK(info->st_mode)) printf("File type: Symbolic link\n");
    else if (S_ISCHR(info->st_mode)) printf("File type: Character device\n");
    else if (S_ISBLK(info->st_mode)) printf("File type: Block device\n");
    else if (S_ISFIFO(info->st_mode))printf("File type: FIFO (pipe)\n");
    else if (S_ISSOCK(info->st_mode))printf("File type: Socket\n");
    else                             printf("File type: Unknown\n");
}
