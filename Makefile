BIN = bin
LIB = lib
OBJ = obj
SRC = src

CC = gcc
CFLAGS = -Wall

$(BIN)/client: $(SRC)/client.c $(OBJ)/fonctionsTCP.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/serveur: $(SRC)/serveur.c $(OBJ)/fonctionsTCP.o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/fonctionsTCP.o: $(SRC)/fonctionsTCP.c $(LIB)/fonctionsTCP.h
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f bin/client bin/serveur
