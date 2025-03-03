# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := restaurant

BUILD_DIR := ./build
SRC_DIRS := ./simulator

CC := gcc

CFLAGS = -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-result -Wno-unused-but-set-variable -Wno-pointer-arith -Werror -std=c17 -Wpedantic -O0 -g -pthread

SRCS := $(shell find $(SRC_DIRS) -name '*.c')

# ./dir/hello.cpp into ./build/./dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC)  $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
