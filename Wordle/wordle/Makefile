CC = gcc
CFLAGS = -Wall -Wfatal-errors -g
LDFLAGS = -pthread
OBJECTS = mots_5_lettres.o wordle_serveur.o
OBJECTS_CLT = wordle_client.o mots_5_lettres.o
PROGS = wordle_srv
PROGS_CLT = wordle_clt
all: $(PROGS) $(PROGS_CLT)

wordle_srv: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)
wordle_clt: $(OBJECTS_CLT)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS_CLT)

mots_5_lettres.o: mots_5_lettres.c mots_5_lettres.h
wordle_simple.o: wordle_serveur.c mots_5_lettres.h
wordle_client.o: wordle_client.c

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(PROGS)
