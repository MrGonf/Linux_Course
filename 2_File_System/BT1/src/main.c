#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main (void){
    int fd; // file desriptor
    int numb_read, numb_writes;
    char buf[12] = "hello world\n";

    fd = open("./text/hello.txt", O_WRONLY | O_APPEND) ;

    if (fd == -1){
        printf("Write %d bytes to hello.txt\n", numb_writes);
    }

    numb_writes = write(fd, buf, strlen(buf));

    lseek(fd, 6, SEEK_SET); // Move file pointer to the 7th byte from the start of the file
    write(fd, "NEW_TEXT", strlen("NEW_TEXT"));
    close(fd);

    return 0;
}