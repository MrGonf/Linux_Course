#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#define BUFF_SIZE 256
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Global variables
int server_fd; // Server socket
struct sockaddr_in client_addr; // Client address for sending messages
socklen_t client_len = sizeof(client_addr); // Client address length
volatile int running = 1; // Flag to control thread loops
volatile int client_connected = 0; // Flag to indicate client has sent first message

// Thread function to receive messages from client
void *receive_thread(void *arg) {
    char recvbuff[BUFF_SIZE];
    int numb_read;

    while (running) {
        // Clear receive buffer
        memset(recvbuff, 0, BUFF_SIZE);
        // Receive message from client
        numb_read = recvfrom(server_fd, recvbuff, BUFF_SIZE, 0, 
                            (struct sockaddr *)&client_addr, &client_len);
        if (numb_read < 0) {
            perror("recvfrom()");
            running = 0;
            break;
        }
        // Set client_connected flag after receiving first message
        client_connected = 1;
        // Check for exit command
        if (strncmp(recvbuff, "exit", 4) == 0) {
            printf("Client sent exit command. Closing connection.\n");
            running = 0;
            break;
        }
        printf("Message from client: %s\n", recvbuff);
    }
    return NULL;
}

// Thread function to send messages to client
void *send_thread(void *arg) {
    char sendbuff[BUFF_SIZE];
    int numb_write;

    while (running) {
        // Prompt user for message
        printf("Please enter the message to client (type 'exit' to quit): ");
        // Clear send buffer
        memset(sendbuff, 0, BUFF_SIZE);
        // Get input from user
        fgets(sendbuff, BUFF_SIZE, stdin);
        // Remove newline character
        sendbuff[strcspn(sendbuff, "\n")] = 0;

        // Send message only if client has connected
        if (client_connected) {
            numb_write = sendto(server_fd, sendbuff, strlen(sendbuff), 0, 
                               (struct sockaddr *)&client_addr, client_len);
            if (numb_write < 0) {
                perror("sendto()");
                running = 0;
                break;
            }
            // Check for exit command
            if (strncmp(sendbuff, "exit", 4) == 0) {
                printf("Server exit.\n");
                running = 0;
                break;
            }
        } else {
            printf("Waiting for client to send first message...\n");
            sleep(1); // Avoid busy waiting
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    int portno;
    pthread_t send_tid, receive_tid;

    // Check command-line arguments
    if (argc < 2) {
        printf("command: %s <port number>\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[1]);
    // Validate port number
    if (portno < 1024 || portno > 65535) {
        printf("Invalid port number. Use a port between 1024 and 65535.\n");
        exit(EXIT_FAILURE);
    }

    // Create socket (IPv4, UDP)
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        handle_error("socket()");
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept any IP
    server_addr.sin_port = htons(portno); // Convert port to network byte order

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        handle_error("bind()");
    }
    printf("Server listening on port %d...\n", portno);

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
    printf("Server terminated.\n");
    return 0;
}