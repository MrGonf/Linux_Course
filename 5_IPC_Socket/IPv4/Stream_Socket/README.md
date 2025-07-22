# TCP Socket Chat Application

This project implements a simple client-server chat application using **IPv4 Stream Sockets** (TCP) on Linux. The server can proactively send messages to the client without waiting for the client to initiate, and the connection persists until either side sends the "exit" command. The application uses multithreading (pthread) to handle sending and receiving messages concurrently.

## Prerequisites

To build and run this application, ensure you have:
- **Linux OS** (e.g., Ubuntu, Debian, or WSL on Windows).
- **GCC Compiler**: Install with `sudo apt update && sudo apt install build-essential`.
- **pthread Library**: Included with GCC for multithreading support.
- **Terminal**: For compiling and running the program.
- **Text Editor**: VS Code, Vim, or any editor for writing code.

## Files

- `server.c`: Server program that listens for client connections and handles bidirectional communication.
- `client.c`: Client program that connects to the server and exchanges messages.

## Steps to Code the Application

Below are the detailed steps to implement the TCP socket chat application, including the key functions used from `<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`, `<unistd.h>`, and `<pthread.h>`.

### 1. Initialize Socket
- **Purpose**: Create a socket for network communication.
- **Function**: `socket(AF_INET, SOCK_STREAM, 0)`
  - `AF_INET`: Specifies IPv4 protocol.
  - `SOCK_STREAM`: Specifies TCP (reliable, stream-based).
  - Returns a file descriptor (`server_fd` for server, `sock` for client) or -1 on error.
- **Code Example**:
  ```c
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
      perror("socket()");
      exit(EXIT_FAILURE);
  }
  ```

### 2. Configure Server Address
- **Purpose**: Set up the IP address and port for the server to listen on or the client to connect to.
- **Functions**:
  - `memset(&server_addr, 0, sizeof(server_addr))`: Clear the address structure.
  - `server_addr.sin_family = AF_INET`: Set protocol family to IPv4.
  - `server_addr.sin_addr.s_addr = INADDR_ANY`: Allow connections from any IP (server).
  - `server_addr.sin_port = htons(portno)`: Convert port to network byte order.
  - `inet_pton(AF_INET, ip_str, &serv_addr.sin_addr)`: Convert IP string to binary (client).
- **Code Example (Server)**:
  ```c
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(portno);
  ```

### 3. Bind and Listen (Server Only)
- **Purpose**: Bind the socket to an address/port and set it to listen for connections.
- **Functions**:
  - `bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))`: Bind socket to address.
  - `listen(server_fd, 3)`: Set socket to listen mode (3 is the backlog for pending connections).
- **Code Example**:
  ```c
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
      perror("bind()");
      exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0) {
      perror("listen()");
      exit(EXIT_FAILURE);
  }
  ```

### 4. Accept Connection (Server Only)
- **Purpose**: Accept incoming client connections.
- **Function**: `accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)`
  - Returns a new socket (`client_fd`) for communication with the client.
- **Code Example**:
  ```c
  struct sockaddr_in client_addr;
  int addr_len = sizeof(client_addr);
  int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len);
  if (client_fd < 0) {
      perror("accept()");
      exit(EXIT_FAILURE);
  }
  ```

### 5. Connect to Server (Client Only)
- **Purpose**: Establish a connection to the server.
- **Function**: `connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))`
- **Code Example**:
  ```c
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("connect()");
      exit(EXIT_FAILURE);
  }
  ```

### 6. Implement Bidirectional Communication with Multithreading
- **Purpose**: Allow both server and client to send and receive messages concurrently, with the server able to send messages proactively.
- **Functions**:
  - `pthread_create(&tid, NULL, thread_func, NULL)`: Create threads for sending and receiving.
  - `pthread_join(tid, NULL)`: Wait for threads to finish.
  - `read(fd, buffer, size)`: Read data from socket.
  - `write(fd, buffer, strlen(buffer))`: Send data through socket.
  - `fgets(buffer, size, stdin)`: Get user input.
  - `strcspn(buffer, "\n")`: Remove newline from input.
  - `strncmp(buffer, "exit", 4)`: Check for exit command.
  - `memset(buffer, 0, size)`: Clear buffer.
- **Code Example (Thread Functions)**:
  ```c
  void *receive_thread(void *arg) {
      char recvbuff[BUFF_SIZE];
      int numb_read;
      while (running) {
          memset(recvbuff, 0, BUFF_SIZE);
          numb_read = read(client_fd, recvbuff, BUFF_SIZE);
          if (numb_read <= 0) {
              running = 0;
              break;
          }
          if (strncmp(recvbuff, "exit", 4) == 0) {
              running = 0;
              break;
          }
          printf("Message from client: %s\n", recvbuff);
      }
      return NULL;
  }
  ```

### 7. Handle Connection Termination
- **Purpose**: Close the connection when "exit" is received or a disconnection occurs.
- **Functions**:
  - `strncmp(buffer, "exit", 4)`: Check for exit command.
  - Set `running = 0` to stop threads.
- **Code Example**:
  ```c
  if (strncmp(sendbuff, "exit", 4) == 0) {
      printf("Client exit...\n");
      running = 0;
      break;
  }
  ```

### 8. Close Sockets
- **Purpose**: Release socket resources.
- **Function**: `close(fd)`
- **Code Example**:
  ```c
  close(client_fd);
  close(server_fd);
  ```

## Compilation and Execution

1. **Compile the Programs**:
   ```bash
   gcc server.c -o server -pthread
   gcc client.c -o client -pthread
   ```
   The `-pthread` flag links the pthread library for multithreading.

2. **Run the Server**:
   ```bash
   ./server 8080
   ```
   Replace `8080` with your desired port (1024–65535).

3. **Run the Client**:
   ```bash
   ./client 127.0.0.1 8080
   ```
   Replace `127.0.0.1` with the server IP and `8080` with the same port used by the server.

4. **Interact**:
   - The server can send messages immediately after the client connects.
   - Both server and client can send messages at any time.
   - Type "exit" to terminate the connection.

## Example Output

**Server**:
```
Server listening on port 8080...
Client connected.
Please enter the message to client (type 'exit' to quit): Hello Client!
Message from client: Hi Server!
Please enter the message to client (type 'exit' to quit): exit
Server exit.
Server terminated.
```

**Client**:
```
Connected to server.
Message from Server: Hello Client!
Please enter the message (type 'exit' to quit): Hi Server!
Message from Server: exit
Server exit...
Client terminated.
```

## Troubleshooting

- **"Address already in use" Error**:
  Add the following before `bind()` in `server.c` to reuse the port:
  ```c
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  ```
- **Debugging**:
  Use `gdb` (`sudo apt install gdb`) or `valgrind` (`sudo apt install valgrind`) to debug:
  ```bash
  gdb ./server
  valgrind ./server
  ```
- **Check Ports**:
  Use `ss -tuln | grep 8080` to check if the port is in use.

## Learning Resources

- **Beej’s Guide to Network Programming**: http://beej.us/guide/bgnet/
- **Linux Man Pages**: `man socket`, `man bind`, `man listen`, `man accept`, `man connect`
- **Book**: "UNIX Network Programming" by W. Richard Stevens
- **Online Courses**: Look for "Socket Programming in C" on Coursera or Udemy
- **Community**: Stack Overflow (tags: socket, c, linux), Reddit (r/C_Programming)