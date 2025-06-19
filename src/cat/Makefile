CFLAGS = -Wall -Wextra -Werror -std=c11

OBJ_DIR = obj
CAT_SRCS = $(wildcard *.c)

TARGET = s21_cat
CAT_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(CAT_SRCS))

all: $(TARGET)

$(TARGET): $(CAT_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean