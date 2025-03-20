CC := gcc

CFLAGS = -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-result -Wno-unused-but-set-variable -Wno-pointer-arith  -std=c17 -Wpedantic -O0 -g -pthread

TARGET_EXEC := restaurant
TEST_TARGET := test

BUILD_DIR := ./build
SRC_DIRS := ./simulator
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
# ./dir/hello.cpp into ./build/./dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

TEST_SRC_DIRS := ./tests ./simulator 
TEST_SRC := $(filter-out %sim/main.c, $(shell find $(TEST_SRC_DIRS) -name '*.c'))
TEST_OBJS := $(TEST_SRC:%=$(BUILD_DIR)/%.o)



$(TARGET_EXEC): $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC)  $(CFLAGS) $(DFLAGS) -c $< -o $@

$(TEST_TARGET): $(BUILD_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $^ -o $@ 

run_tests: $(BUILD_DIR)/$(TEST_TARGET)
	./build/test

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm logs/*
