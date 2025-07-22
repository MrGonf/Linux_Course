# ================================
# Compiler and Flags
# ================================
CC       = gcc
CFLAGS   = -Wall -g -pthread

ifdef DEBUG
  CFLAGS += -DDEBUG
endif

# ================================
# Directories
# ================================
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

# ================================
# Target Executables
# ================================
SERVER_TARGET = $(BINDIR)/server
CLIENT_TARGET = $(BINDIR)/client

# ================================
# Source and Object files
# ================================
SERVER_SRC   = $(SRCDIR)/server.c
CLIENT_SRC   = $(SRCDIR)/client.c
SERVER_OBJ   = $(OBJDIR)/server.o
CLIENT_OBJ   = $(OBJDIR)/client.o

# ================================
# Default target
# ================================
all: prep $(SERVER_TARGET) $(CLIENT_TARGET)

# ================================
# Create necessary directories
# ================================
prep:
	@mkdir -p $(OBJDIR) $(BINDIR)

# ================================
# Compile .c to .o
# ================================
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# ================================
# Link objects to executables
# ================================
$(SERVER_TARGET): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful: $@"

$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful: $@"

# ================================
# Run the server
# ================================
run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# ================================
# Run the client
# ================================
run-client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)

# ================================
# Clean build artifacts
# ================================
clean:
	rm -rf $(OBJDIR) $(BINDIR) /tmp/chat_socket

# ================================
# Rebuild from scratch
# ================================
rebuild: clean all

# ================================
# Phony targets
# ================================
.PHONY: all run-server run-client clean rebuild prep