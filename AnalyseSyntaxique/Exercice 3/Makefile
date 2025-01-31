# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable
TARGET = main

# Source files
SRCS = analex.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean