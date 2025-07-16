#include <stdio.h>
#include <fcntl.h>     // For open, O_RDONLY, O_WRONLY, O_APPEND, O_CREAT
#include <unistd.h>    // For read, write, close
#include <string.h>    // For strlen
#include "func.h"
/*
 * READ FUNCTION
 */
int read_func(const char *filename, int num_bytes, char *buffer, size_t buffer_size) {
    // Open file in read-only mode
    int fd = open(filename, O_RDONLY);
    
    if (fd == -1) {
        perror("Error opening file for reading");
        return -1;
    }

    // Read up to num_bytes or buffer_size, whichever is smaller
    ssize_t bytes_read = read(fd, buffer, (num_bytes < buffer_size) ? num_bytes : buffer_size);

    if (bytes_read == -1) {
        perror("Error reading from file");
        close(fd);
        return -1;
    }

    // Null-terminate the buffer for printing
    buffer[bytes_read] = '\0';
    printf("Read %zd bytes: %s\n", bytes_read, buffer);

    // Close the file
    close(fd);
    return 0;
}

/*
 * WRITE FUNCTION
 */
int write_func(const char *filename, const char *content, int num_bytes) {
    // Open file in write-only mode with O_APPEND and O_CREAT
    int fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);

    if (fd == -1) {
        perror("Error opening file for writing");
        return -1;
    }

    // Calculate bytes to write (limited by num_bytes and content length)
    size_t content_len = strlen(content);
    size_t bytes_to_write = (content_len < num_bytes) ? content_len : num_bytes;

    // Write up to num_bytes of content
    ssize_t bytes_written = write(fd, content, bytes_to_write);

    if (bytes_written == -1) {
        perror("Error writing to file");
        close(fd);
        return -1;
    }

    printf("Wrote %zd bytes to %s\n", bytes_written, filename);

    // Close the file
    close(fd);
    return 0;
}
