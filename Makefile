TARGET = player

# Source files
SRCS = main.c

# Compiler and flags
CC = gcc

# Use pkg-config to get GTK4 flags
GTK_CFLAGS = $(shell pkg-config --cflags gtk4)
GTK_LIBS   = $(shell pkg-config --libs gtk4)

# Default target: build the program
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(GTK_CFLAGS) $(GTK_LIBS)

# Clean up build artifacts
clean:
	rm -f $(TARGET)

# Generate compile_commands.json using Bear
compile_commands.json: clean
	bear -- $(CC) $(SRCS) -o $(TARGET) $(GTK_CFLAGS) $(GTK_LIBS)

.PHONY: all clean
