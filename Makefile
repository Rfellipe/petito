# Compiler and flags
CC = gcc
CFLAGS = -I./include
LDFLAGS = -lssl -lcrypto

# Directories
LIB_DIR = lib
INCLUDE_DIR = include
OBJ_DIR = obj

# Recursively find all .c files in LIB_DIR and its subdirectories
SRC_FILES = $(shell find $(LIB_DIR) -type f -name '*.c')

# Generate corresponding object files maintaining directory structure
OBJ_FILES = $(patsubst $(LIB_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Recursively find all subdirectories in include
INCLUDE_DIRS = $(shell find $(INCLUDE_DIR) -type d)
CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

# Output executable
TARGET = petito

# Rule to compile everything
all: $(TARGET)

# Rule to link the object files with main.c
$(TARGET): $(OBJ_FILES) main.o
	$(CC) -o $(TARGET) main.o $(OBJ_FILES) $(LDFLAGS)

# Rule to compile main.c
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Rule to compile source files
$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up object files and executable
clean:
	rm -rf $(OBJ_DIR) *.o $(TARGET)

.PHONY: all clean
