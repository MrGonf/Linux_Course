# =============================
# Compiler and Flags
# =============================
CC      = gcc
CFLAGS  = -Wall -g
INCDIR  = inc
INCLUDES = -I$(INCDIR)

# =============================
# Directories
# =============================
SRCDIR  = src
OBJDIR  = obj
TXTDIR  = txt

# =============================
# Target Executable
# =============================
TARGET  = 2_BT2

# =============================
# Auto-detect Source and Object Files
# =============================
SRCS    = $(wildcard $(SRCDIR)/*.c)
OBJS    = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# =============================
# Default Target
# =============================
all: $(TARGET) init_text

# =============================
# Rule to compile .c -> .o
# =============================
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# =============================
# Rule to link object files into binary
# =============================
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# =============================
# Create txt/text.txt if not exists
# =============================
init_text:
	@mkdir -p $(TXTDIR)
	@touch $(TXTDIR)/text.txt

# =============================
# Run the executable with arguments
# =============================
run: $(TARGET)
	./$(TARGET) $(TXTDIR)/text.txt 5 w "Hello"

# =============================
# Clean up
# =============================
clean:
	rm -f $(TARGET) $(OBJS) $(TXTDIR)/text.txt

# =============================
# Phony targets
# =============================
.PHONY: all run clean init_text
