#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define BUFF_SIZE 256
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Global variable for client socket
int server_fd;
// Flag to control thread loops
volatile int running = 1;

// Thread function to receive messages from server
void *receive_thread(void *arg) {
    char recvbuff[BUFF_SIZE];
    int numb_read;

    while (running) {
        // Clear receive buffer
        memset(recvbuff, 0, BUFF_SIZE);
        // Read data from server
        numb_read = read(server_fd, recvbuff, BUFF_SIZE);
        if (numb_read <= 0) {
            if (numb_read == 0) {
                printf("Server disconnected.\n");
            } else {
                perror("read()");
            }
            running = 0; // Stop threads
            break;
        }
        // Check for exit command
        if (strncmp(recvbuff, "exit", 4) == 0) {
            printf("Server exit...\n");
            running = 0; // Stop threads
            break;
        }
        printf("Message from Server: %s\n", recvbuff);
    }
    return NULL;
}

// Thread function to send messages to server
void *send_thread(void *arg) {
    char sendbuff[BUFF_SIZE];
    int numb_write;

    while (running) {
        // Prompt user for message
        printf("Please enter the message (type 'exit' to quit): ");
        // Clear send buffer
        memset(sendbuff, 0, BUFF_SIZE);
        // Get input from user
        fgets(sendbuff, BUFF_SIZE, stdin);
        // Remove newline character
        sendbuff[strcspn(sendbuff, "\n")] = 0;

        // Send message to server
        numb_write = write(server_fd, sendbuff, strlen(sendbuff));
        if (numb_write < 0) {
            perror("write()");
            running = 0; // Stop threads
            break;
        }
        // Check for exit command
        if (strncmp(sendbuff, "exit", 4) == 0) {
            printf("Client exit...\n");
            running = 0; // Stop threads
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int portno;
    struct sockaddr_in serv_addr;
    pthread_t send_tid, receive_tid;

    // Check command-line arguments
    if (argc < 3) {
        printf("command: %s <server address> <port number>\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[2]);
    // Validate port number
    if (portno < 1024 || portno > 65535) {
        printf("Invalid port number. Use a port between 1024 and 65535.\n");
        exit(EXIT_FAILURE);
    }

    // Create socket (IPv4, TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        handle_error("socket()");
    }

    // Configure server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno); // Convert port to network byte order
    // Convert IP address from string to binary
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        handle_error("inet_pton()");
    }

    // Connect to server
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        handle_error("connect()");
    }
    printf("Connected to server.\n");

    // Create threads for sending and receiving
    if (pthread_create(&receive_tid, NULL, receive_thread, NULL) != 0) {
        handle_error("pthread_create(receive)");
    }
    if (pthread_create(&send_tid, NULL, send_thread, NULL) != 0) {
        handle_error("pthread_create(send)");
    }

    // Wait for threads to finish
    pthread_join(receive_tid, NULL);
    pthread_join(send_tid, NULL);

    // Close socket
    close(server_fd);
    printf("Client terminated.\n");
    return 0;
}