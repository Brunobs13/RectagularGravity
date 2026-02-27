CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
INCLUDES := -Iinclude
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

CORE_SRCS := src/core/rect_world.c
CLI_SRCS := src/cli/main.c

CORE_OBJS := $(CORE_SRCS:src/%.c=$(OBJ_DIR)/%.o)
CLI_OBJS := $(CLI_SRCS:src/%.c=$(OBJ_DIR)/%.o)

TARGET := $(BUILD_DIR)/rectangle_gravity
TEST_BIN := $(BUILD_DIR)/test_rect_world

.PHONY: all build test clean run web-run

all: build

build: $(TARGET)

$(TARGET): $(CORE_OBJS) $(CLI_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_BIN): tests/test_rect_world.c src/core/rect_world.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

run: $(TARGET)
	./$(TARGET) create 10,8+8,4 create 12,16+6,3 moveright 10,1+2

web-run:
	python3 -m http.server 8080 --directory web

clean:
	rm -rf $(BUILD_DIR)
