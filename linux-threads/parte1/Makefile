CC=gcc
CFLAGS=-O3 -march=native -Wall -Wextra
LDFLAGS=-lpthread -lm

BIN=bin
BUILD=build
SRC_DIR=src

TARGET=$(BIN)/threads_parte1
SRCS=$(SRC_DIR)/threads_parte1.c
OBJS=$(BUILD)/threads_parte1.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN) $(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BIN) $(BUILD)
