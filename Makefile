# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Source files
SRC = main.c chunk.c memory.c debug.c value.c vm.c

# Header files
HEADERS = common.h chunk.h memory.h debug.h value.h vm.h

# Object files (one per source file)
OBJ = $(SRC:.c=.o)

# Output executable
TARGET = clox

# Default target
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each source file into an object file
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild the entire project
rebuild: clean all