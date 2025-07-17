#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include "func.h"

int main() {
    const char *filename = "./txt/text.txt";
    const char *data = "This is example data written to the file.\n";

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }
    if (write(fd, data, strlen(data)) == -1) {
        perror("Failed to write data");
        close(fd);
        return EXIT_FAILURE;
    }
    close(fd);

    // Get file infor
    struct stat file_info;
    if (stat(filename, &file_info) == -1) {
        perror("Failed to stat file");
        return EXIT_FAILURE;
    }

    print_file_infor(&file_info, filename);

    return EXIT_SUCCESS;
}
