CC = gcc
CFLAGS = -Wall -Wextra -g -pedantic -lm
SRC_DIR = src
BIN_DIR = build
TARGET = $(BIN_DIR)/hash_table

TEST_DIR = tests
TEST_TARGET = $(BIN_DIR)/hash_table_test

# List your source files (replace with your actual source files)
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRC_FILES))
TEST_FILES = $(wildcard $(TEST_DIR)/*.c,$(SRC_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%.o,$(TEST_FILES))

# Default target
all: $(TARGET)


# Rule to create object files from source files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create object files from test source files
$(BIN_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link object files into the final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_TARGET)

# Rule to create object files from source files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create object files from test source files
$(BIN_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link object files into the final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to link test object files into the final test executable
$(TEST_TARGET): $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to clean the generated files
clean:
	rm -rf $(BIN_DIR)

.PHONY: all test clean
