# Unix Stream Socket Chat Application

This project implements a client-server chat application using **Unix Stream Sockets** (AF_UNIX, SOCK_STREAM) on Linux. The server can proactively send messages to the client after connection, and communication continues until either side sends the "exit" command. Multithreading (pthread) is used to handle sending and receiving messages concurrently. All code includes English comments for clarity.

## Prerequisites

- **Linux OS**: Ubuntu, Debian, or WSL on Windows.
- **GCC Compiler**: Install with `sudo apt update && sudo apt install build-essential`.
- **pthread Library**: Included with GCC for multithreading.
- **Make**: Install with `sudo apt install make` for building with Makefile.
- **Terminal**: For compiling and running the programs.
- **Text Editor**: VS Code, Vim, or any editor for coding.

## Directory Structure

```
unix_socket_chat/
├── src/
│   ├── server.c    # Server source code (Unix Stream Socket)
│   ├── client.c    # Client source code (Unix Stream Socket)
├── obj/            # Object files (auto-generated)
├── bin/            # Executable files (auto-generated)
├── Makefile        # Build script
├── README.md       # This file
```

## Steps to Code the Application

Below are the steps to implement the Unix Stream Socket chat application, with key functions from `<sys/socket.h>`, `<sys/un.h>`, `<unistd.h>`, and `<pthread.h>`.

### 1. Initialize Socket
- **Purpose**: Create a Unix Stream socket for communication.
- **Function**: `socket(AF_UNIX, SOCK_STREAM, 0)`
  - `AF_UNIX`: Unix domain protocol (local communication).
  - `SOCK_STREAM`: TCP-based, reliable, connection-oriented.
  - Returns a file descriptor or -1 on error.
- **Code Example** (server.c):
  ```c
  int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_fd < 0) {
      perror("socket()");
      exit(EXIT_FAILURE);
  }
  ```

### 2. Configure Server Address
- **Purpose**: Set up the socket file path for server or client.
- **Functions**:
  - `memset(&server_addr, 0, sizeof(server_addr))`: Clear address structure.
  - `server_addr.sun_family = AF_UNIX`: Set Unix domain protocol.
  - `strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1)`: Set socket file path.
- **Code Example** (server.c):
  ```c
  struct sockaddr_un server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
  ```

### 3. Bind and Listen (Server Only)
- **Purpose**: Bind the socket to a file path and listen for connections.
- **Functions**:
  - `unlink(SOCKET_PATH)`: Remove existing socket file.
  - `bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))`: Bind socket to file path.
  - `listen(server_fd, 3)`: Set socket to listen mode.
- **Code Example**:
  ```c
  unlink(SOCKET_PATH);
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
- **Code Example**:
  ```c
  client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
  if (client_fd < 0) {
      perror("accept()");
      exit(EXIT_FAILURE);
  }
  ```

### 5. Connect to Server (Client Only)
- **Purpose**: Connect to the server's socket file.
- **Function**: `connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))`
- **Code Example**:
  ```c
  if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
      perror("connect()");
      exit(EXIT_FAILURE);
  }
  ```

### 6. Implement Bidirectional Communication with Multithreading
- **Purpose**: Allow both server and client to send and receive messages concurrently, with the server able to send messages proactively.
- **Functions**:
  - `pthread_create(&tid, NULL, thread_func, NULL)`: Create threads for sending and receiving.
  - `pthread_join(tid, NULL)`: Wait for threads to finish.
  - `read(fd, buffer, len)`: Read message from socket.
  - `write(fd, buffer, len)`: Send message through socket.
  - `fgets(buffer, size, stdin)`: Get user input.
  - `strcspn(buffer, "\n")`: Remove newline from input.
  - `strncmp(buffer, "exit", 4)`: Check for exit command.
  - `memset(buffer, 0, size)`: Clear buffer.
- **Code Example** (server.c, send_thread):
  ```c
  numb_write = write(client_fd, sendbuff, strlen(sendbuff));
  ```

### 7. Handle Connection Termination
- **Purpose**: Stop communication when "exit" is received or an error occurs.
- **Functions**:
  - `strncmp(buffer, "exit", 4)`: Check for exit command.
  - Set `running = 0` to stop threads.
- **Code Example**:
  ```c
  if (strncmp(sendbuff, "exit", 4) == 0) {
      printf("Server exit.\n");
      running = 0;
      break;
  }
  ```

### 8. Close Socket and Clean Up
- **Purpose**: Release socket resources and remove socket file.
- **Functions**:
  - `close(fd)`: Close socket.
  - `unlink(SOCKET_PATH)`: Remove socket file (server only).
- **Code Example**:
  ```c
  close(client_fd);
  close(server_fd);
  unlink(SOCKET_PATH);
  ```

## Compilation and Execution

1. **Compile the Programs**:
   ```bash
   make
   ```
   Creates `bin/server` and `bin/client`.

2. **Run the Server**:
   ```bash
   make run-server
   ```
   Uses socket file `/tmp/chat_socket`.

3. **Run the Client**:
   ```bash
   make run-client
   ```
   Connects to `/tmp/chat_socket`.

4. **Interact**:
   - The server can send messages immediately after the client connects.
   - Both server and client can send messages at any time.
   - Type "exit" to terminate the connection.

## Example Output

**Server**:
```
Server listening on socket /tmp/chat_socket...
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
  Ensure the socket file is removed before binding:
  ```c
  unlink(SOCKET_PATH);
  ```
  Or run `make clean` to remove `/tmp/chat_socket`.
- **Debugging**:
  Use `gdb` or `valgrind`:
  ```bash
  gdb ./bin/server
  valgrind ./bin/server
  ```
- **Check Socket File**:
  ```bash
  ls -l /tmp/chat_socket
  ```

## Learning Resources

- **Beej’s Guide to Network Programming**: http://beej.us/guide/bgnet/ (Unix sockets section)
- **Linux Man Pages**: `man socket`, `man unix`, `man bind`, `man listen`, `man accept`, `man connect`
- **Book**: "UNIX Network Programming" by W. Richard Stevens
- **Online Courses**: Search for "Socket Programming in C" on Coursera or Udemy
- **Community**: Stack Overflow (tags: socket, unix, c), Reddit (r/C_Programming)
