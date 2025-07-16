# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Werror -Iinclude
LDFLAGS = -lpthread -lm

# Directories
SRCDIR = src
OBJDIR = obj

# Executable name
TARGET = pzip

# Find all source files recursively
SOURCES := $(shell find $(SRCDIR) -name '*.c')

# Create object file paths, preserving directory structure
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Get all unique directories that need to be created in OBJDIR
OBJDIRS := $(sort $(dir $(OBJECTS)))

# Default target
all: $(TARGET)

# Create the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJDIR) $(TARGET)
	
# Run tests
test:
	cp pzip tests/pzip && cd tests && ./tester/run-tests.sh $* && rm pzip &&cd ..

# Debug target to show variables
debug:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "OBJDIRS: $(OBJDIRS)"

.PHONY: all clean debug
