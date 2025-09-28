CC=gcc
CFLAGS_PARTE1=-O3 -march=native -Wall -Wextra
CFLAGS_PARTE2=-O2 -Wall -Wextra
LDFLAGS=-lpthread -lm

BIN=bin
BUILD=build

.PHONY: all parte1 parte2 clean

all: parte1 parte2

parte1: $(BIN)/threads_parte1

parte2: $(BIN)/threads_parte2

$(BIN)/threads_parte1: $(BUILD)/threads_parte1.o | $(BIN)
	$(CC) $(CFLAGS_PARTE1) -o $@ $< $(LDFLAGS)

$(BIN)/threads_parte2: $(BUILD)/threads_parte2.o | $(BIN)
	$(CC) $(CFLAGS_PARTE2) -o $@ $< -lpthread

$(BUILD)/threads_parte1.o: parte1/src/threads_parte1.c | $(BUILD)
	$(CC) $(CFLAGS_PARTE1) -c $< -o $@

$(BUILD)/threads_parte2.o: parte2/src/threads_parte2.c | $(BUILD)
	$(CC) $(CFLAGS_PARTE2) -c $< -o $@

$(BIN) $(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BIN) $(BUILD)
