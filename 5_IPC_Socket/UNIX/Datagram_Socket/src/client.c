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

// Global variables
int sock_fd; // Client socket
struct sockaddr_in server_addr; // Server address for sending messages
socklen_t server_len = sizeof(server_addr); // Server address length
volatile int running = 1; // Flag to control thread loops

// Thread function to receive messages from server
void *receive_thread(void *arg) {
    char recvbuff[BUFF_SIZE];
    int numb_read;

    while (running) {
        // Clear receive buffer
        memset(recvbuff, 0, BUFF_SIZE);
        // Receive message from server
        numb_read = recvfrom(sock_fd, recvbuff, BUFF_SIZE, 0, NULL, NULL);
        if (numb_read < 0) {
            perror("recvfrom()");
            running = 0;
            break;
        }
        // Check for exit command
        if (strncmp(recvbuff, "exit", 4) == 0) {
            printf("Server exit...\n");
            running = 0;
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
        numb_write = sendto(sock_fd, sendbuff, strlen(sendbuff), 0, 
                           (struct sockaddr *)&server_addr, server_len);
        if (numb_write < 0) {
            perror("sendto()");
            running = 0;
            break;
        }
        // Check for exit command
        if (strncmp(sendbuff, "exit", 4) == 0) {
            printf("Client exit...\n");
            running = 0;
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int portno;
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

    // Create socket (IPv4, UDP)
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        handle_error("socket()");
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno); // Convert port to network byte order
    // Convert IP address from string to binary
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        handle_error("inet_pton()");
    }

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