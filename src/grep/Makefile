CFLAGS = -Wall -Wextra -Werror -std=c11

OBJ_DIR = obj
GREP_SRCS = $(wildcard *.c)

TARGET = s21_grep
GREP_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(GREP_SRCS))

all: $(TARGET)

$(TARGET): $(GREP_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean