#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

#define BUFF_SIZE 256
#define SOCKET_PATH "/tmp/chat_socket"
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Global variable for client socket
int sock_fd;
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
        numb_read = read(sock_fd, recvbuff, BUFF_SIZE);
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
        numb_write = write(sock_fd, sendbuff, strlen(sendbuff));
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

int main(void) {
    struct sockaddr_un server_addr;
    pthread_t send_tid, receive_tid;

    // Create Unix Stream socket
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        handle_error("socket()");
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Connect to server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
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
    close(sock_fd);
    printf("Client terminated.\n");
    return 0;
}
