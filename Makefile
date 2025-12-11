CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -D_GNU_SOURCE -Iinclude
LDFLAGS = -lncurses -lm

SRC_DIR = src
OBJ_DIR = build

# Find all source files in src/
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Add main.c
SRCS += main.c

# Generate object file names
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

TARGET = hms

.PHONY: all clean debug release

all: debug

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

release: CFLAGS += -O3 -DNDEBUG
release: $(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) data/*.hms

run: $(TARGET)
	./$(TARGET)
