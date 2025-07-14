# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Werror -Iinclude
LDFLAGS = -lpthread -lm

# AddressSanitizer flags
ASAN_CFLAGS = $(CFLAGS) -fsanitize=address -fno-omit-frame-pointer
ASAN_LDFLAGS = $(LDFLAGS) -fsanitize=address

# Directories
SRCDIR = src
OBJDIR = obj
ASAN_OBJDIR = obj_asan

# Executable name
TARGET = pzip
ASAN_TARGET = pzip_asan

# Find all source files recursively
SOURCES := $(shell find $(SRCDIR) -name '*.c')

# Create object file paths, preserving directory structure
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ASAN_OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(ASAN_OBJDIR)/%.o)

# Get all unique directories that need to be created in OBJDIR
OBJDIRS := $(sort $(dir $(OBJECTS)))
ASAN_OBJDIRS := $(sort $(dir $(ASAN_OBJECTS)))

# Default target
all: $(TARGET)

# AddressSanitizer target
asan: $(ASAN_TARGET)

# Create the regular executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Create the AddressSanitizer executable
$(ASAN_TARGET): $(ASAN_OBJECTS)
	$(CC) $(ASAN_CFLAGS) -o $@ $^ $(ASAN_LDFLAGS)

# Compile source files to object files (regular)
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files to object files (AddressSanitizer)
$(ASAN_OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(ASAN_CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJDIR) $(ASAN_OBJDIR) $(TARGET) $(ASAN_TARGET)

# Debug target to show variables
debug:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "ASAN_OBJECTS: $(ASAN_OBJECTS)"
	@echo "OBJDIRS: $(OBJDIRS)"
	@echo "ASAN_OBJDIRS: $(ASAN_OBJDIRS)"

.PHONY: all asan clean debug
