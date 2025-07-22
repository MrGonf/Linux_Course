# UDP Socket Chat Application

This project implements a client-server chat application using **IPv4 Datagram Sockets** (UDP) on Linux. The server can proactively send messages to the client without waiting for the client to initiate, and communication continues until either side sends the "exit" command. Multithreading (pthread) is used to handle sending and receiving messages concurrently. All code includes English comments for clarity.

## Prerequisites

- **Linux OS**: Ubuntu, Debian, or WSL on Windows.
- **GCC Compiler**: Install with `sudo apt update && sudo apt install build-essential`.
- **pthread Library**: Included with GCC for multithreading.
- **Make**: Install with `sudo apt install make` for building with Makefile.
- **Terminal**: For compiling and running the programs.
- **Text Editor**: VS Code, Vim, or any editor for coding.

## Directory Structure

```
udp_socket_chat/
├── src/
│   ├── server.c    # Server source code (UDP)
│   ├── client.c    # Client source code (UDP)
├── obj/            # Object files (auto-generated)
├── bin/            # Executable files (auto-generated)
├── Makefile        # Build script
├── README.md       # This file
```

## Steps to Code the Application

Below are the steps to implement the UDP socket chat application, with key functions from `<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`, `<unistd.h>`, and `<pthread.h>`.

### 1. Initialize Socket
- **Purpose**: Create a UDP socket for communication.
- **Function**: `socket(AF_INET, SOCK_DGRAM, 0)`
  - `AF_INET`: IPv4 protocol.
  - `SOCK_DGRAM`: UDP (datagram-based, connectionless).
  - Returns a file descriptor or -1 on error.
- **Code Example** (server.c):
  ```c
  int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_fd < 0) {
      perror("socket()");
      exit(EXIT_FAILURE);
  }
  ```

### 2. Configure Server Address
- **Purpose**: Set up IP address and port for server or client.
- **Functions**:
  - `memset(&server_addr, 0, sizeof(server_addr))`: Clear address structure.
  - `server_addr.sin_family = AF_INET`: Set IPv4 protocol.
  - `server_addr.sin_addr.s_addr = INADDR_ANY`: Accept any IP (server).
  - `server_addr.sin_port = htons(portno)`: Convert port to network byte order.
  - `inet_pton(AF_INET, ip_str, &server_addr.sin_addr)`: Convert IP string to binary (client).
- **Code Example** (server.c):
  ```c
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(portno);
  ```

### 3. Bind Socket (Server Only)
- **Purpose**: Bind the socket to an address/port.
- **Function**: `bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))`
- **Code Example**:
  ```c
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
      perror("bind()");
      exit(EXIT_FAILURE);
  }
  ```

### 4. Implement Bidirectional Communication with Multithreading
- **Purpose**: Allow both server and client to send and receive messages concurrently, with the server able to send messages proactively.
- **Functions**:
  - `pthread_create(&tid, NULL, thread_func, NULL)`: Create threads for sending and receiving.
  - `pthread_join(tid, NULL)`: Wait for threads to finish.
  - `sendto(fd, buffer, len, 0, dest_addr, addr_len)`: Send message to a specific address.
  - `recvfrom(fd, buffer, len, 0, src_addr, &addr_len)`: Receive message and get source address.
  - `fgets(buffer, size, stdin)`: Get user input.
  - `strcspn(buffer, "\n")`: Remove newline from input.
  - `strncmp(buffer, "exit", 4)`: Check for exit command.
  - `memset(buffer, 0, size)`: Clear buffer.
- **Code Example** (server.c, receive_thread):
  ```c
  numb_read = recvfrom(server_fd, recvbuff, BUFF_SIZE, 0, 
                       (struct sockaddr *)&client_addr, &client_len);
  ```

### 5. Handle Connection Termination
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

### 6. Close Socket
- **Purpose**: Release socket resources.
- **Function**: `close(fd)`
- **Code Example**:
  ```c
  close(server_fd);
  ```

## Compilation and Execution

1. **Compile the Programs**:
   ```bash
   make
   ```
   This creates `bin/server` and `bin/client`.

2. **Run the Server**:
   ```bash
   make run-server
   ```
   Runs server on port 8080. Use `make run-server ARGS="port"` for a custom port.

3. **Run the Client**:
   ```bash
   make run-client
   ```
   Connects to 127.0.0.1:8080. Use `make run-client ARGS="ip port"` for custom IP/port.

4. **Interact**:
   - The server can send messages immediately after receiving the first client message.
   - Both server and client can send messages at any time.
   - Type "exit" to terminate communication.

## Example Output

**Server**:
```
Server listening on port 8080...
Message from client: Hi Server!
Please enter the message to client (type 'exit' to quit): Hello Client!
Message from client: exit
Client sent exit command. Closing connection.
Server terminated.
```

**Client**:
```
Please enter the message (type 'exit' to quit): Hi Server!
Message from Server: Hello Client!
Please enter the message (type 'exit' to quit): exit
Client exit...
Client terminated.
```

## Troubleshooting

- **"Address already in use" Error**:
  Add the following before `bind()` in `server.c`:
  ```c
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  ```
- **Debugging**:
  Use `gdb` or `valgrind`:
  ```bash
  gdb ./bin/server
  valgrind ./bin/server
  ```
- **Check Ports**:
  ```bash
  ss -uln | grep 8080
  ```

## Learning Resources

- **Beej’s Guide to Network Programming**: http://beej.us/guide/bgnet/
- **Linux Man Pages**: `man socket`, `man bind`, `man sendto`, `man recvfrom`
- **Book**: "UNIX Network Programming" by W. Richard Stevens
- **Online Courses**: Search for "Socket Programming in C" on Coursera or Udemy
- **Community**: Stack Overflow (tags: socket, c, linux), Reddit (r/C_Programming)
