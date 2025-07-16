# ================================
# Compiler and Flags
# ================================
CC       = gcc
CFLAGS   = -Wall -g
INCDIR   = inc
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
TARGET   = $(BINDIR)/2BT_3

# ================================
# Source and Object files
# ================================
SRCS     = $(wildcard $(SRCDIR)/*.c)
OBJS     = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# ================================
# Default target
# ================================
all: prep $(TARGET) init_text

# ================================
# Create necessary directories
# ================================
prep:
	@mkdir -p $(OBJDIR) $(TXTDIR) $(BINDIR)

# ================================
# Compile .c -> .o
# ================================
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ================================
# Link objects -> executable
# ================================
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful: $@"

# ================================
# Create txt/text.txt if not exists
# ================================
init_text:
	@touch $(TXTDIR)/text.txt

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
.PHONY: all run clean rebuild prep init_text
