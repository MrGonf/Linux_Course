# ================================
# Compiler and Flags
# ================================
CC       = gcc
CFLAGS   = -Wall -g
INCDIR   = include
INCLUDES = -I$(INCDIR)

ifdef DEBUG
  CFLAGS += -DDEBUG
endif

# ================================
# Directories
# ================================
SRCDIR   = src
OBJDIR   = obj
TXTDIR   = txt
BINDIR   = bin

# ================================
# Target Executable
# ================================
TARGET   = $(BINDIR)/3_BT1

# ================================
# Source and Object files
# ================================
SRCS     = $(wildcard $(SRCDIR)/*.c)
OBJS     = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# ================================
# Default target
# ================================
all: prep $(TARGET)

# ================================
# Create necessary directories
# ================================
prep:
	@mkdir -p $(OBJDIR) $(BINDIR)

# ================================
# Compile .c to .o
# ================================
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ================================
# Link objects to executable
# ================================
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful: $@"


# ================================
# Run the program
# ================================
run: $(TARGET)
	./$(TARGET)

# ================================
# Clean build artifacts
# ================================
clean:
	rm -rf $(OBJDIR) $(TARGET) $(TXTDIR) $(BINDIR)

# ================================
# Rebuild from scratch
# ================================
rebuild: clean all

# ================================
# Phony targets
# ================================
.PHONY: all run clean rebuild prep
