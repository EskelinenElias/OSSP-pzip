# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Iinclude -O # Added -Iinclude for header files
LDFLAGS = -lpthread # Link against pthread and math libraries

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = .  #  Executable in the project root

# Executable name
TARGET = wzip

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link the object files to create the executable
$(TARGET): $(OBJDIR) $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean