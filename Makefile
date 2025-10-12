CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = .
BIN_DIR = bin
INC_DIR = include

CLIENT_SRC = $(SRC_DIR)/client.c $(SRC_DIR)/battle_message.c $(SRC_DIR)/actions_combination.c
SERVER_SRC = $(SRC_DIR)/server.c $(SRC_DIR)/battle_message.c $(SRC_DIR)/actions_combination.c

HEADERS = $(INC_DIR)/battle_message.h $(INC_DIR)/actions_combination.h

CLIENT_BIN = $(BIN_DIR)/client
SERVER_BIN = $(BIN_DIR)/server

all: $(BIN_DIR) $(CLIENT_BIN) $(SERVER_BIN)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(CLIENT_BIN): $(CLIENT_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

$(SERVER_BIN): $(SERVER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

clean:
	rm -rf $(BIN_DIR)

rebuild: clean all

.PHONY: all clean rebuild
