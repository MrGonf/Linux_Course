#include <stdio.h>    
#include <stdlib.h>    
#include <string.h>   
#include "func.h" 

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s filename num-bytes [r/w] content\n", argv[0]);
        return 1;
    }

    // Extract arguments
    char *filename = argv[1];           // File name
    int num_bytes = atoi(argv[2]);      // Number of bytes to read/write
    char *func = argv[3];          // Operation: "r" for read, "w" for write
    char *content = argv[4];            // Content to write or expected content

    // Validate num-bytes
    if (num_bytes < 0) {
        fprintf(stderr, "Error: num-bytes must be non-negative\n");
        return 1;
    }

    // Validate operation
    if (strcmp(func, "r") != 0 && strcmp(func, "w") != 0) {
        fprintf(stderr, "Error: operation must be 'r' or 'w'\n");
        return 1;
    }

    // Handle read or write operation based on the operation type
    if (strcmp(func, "r") == 0) {
        char buffer[1024]; // Temporary buffer for reading
        int result = read_func(filename, num_bytes, buffer, sizeof(buffer));
        if (result == -1) return 1;
    } else if (strcmp(func, "w") == 0) {
        int result = write_func(filename, content, num_bytes);
        if (result == -1) return 1;
    }

    return 0;
}