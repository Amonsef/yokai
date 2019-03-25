BIN = bin
LIB = lib
OBJ = obj
SRC = src

CC = gcc
CFLAGS = -Wall

$(BIN)/client:
	$(CC) $(CFLAGS) -o $@ $(SRC)/client.c

$(BIN)/serveur:
	$(CC) $(CFLAGS) -o $@ $(SRC)/serveur.c

.PHONY: clean
clean:
	rm -f bin/client bin/serveur
