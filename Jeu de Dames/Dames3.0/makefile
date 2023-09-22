all: client serveur CLEAN
piece.o: piece.c piece.h
	gcc -c -Wall piece.c
	
case.o: case.c case.h
	gcc -c -Wall case.c

damier.o: damier.c damier.h
	gcc -c -Wall damier.c

exact_rw.o : exact_rw.c exact_rw.h
	gcc -c -Wall exact_rw.c
	
randomPlayer.o : randomPlayer.c randomPlayer.h
	gcc -c -Wall randomPlayer.c
	
envoi.o : envoi.c envoi.h
	gcc -c -Wall envoi.c
	
adverse.o : adverse.c adverse.h
	gcc -c -Wall adverse.c 
	
enregistrement.o : enregistrement.c enregistrement.h
	gcc -c -Wall enregistrement.c
	

client.o : client.c envoi.h randomPlayer.h adverse.h damier.h enregistrement.h
	gcc -c -Wall client.c
	 
serveur.o : serveur.c envoi.h randomPlayer.h adverse.h damier.h enregistrement.h
	gcc -c -Wall serveur.c
	
	
client: client.o piece.o case.o damier.o exact_rw.o enregistrement.o randomPlayer.o envoi.o adverse.o
	gcc -o client piece.o case.o damier.o exact_rw.o enregistrement.o randomPlayer.o envoi.o adverse.o client.o 

serveur: serveur.o piece.o case.o damier.o exact_rw.o enregistrement.o randomPlayer.o envoi.o adverse.o
	gcc -o serveur piece.o case.o damier.o exact_rw.o enregistrement.o randomPlayer.o envoi.o adverse.o serveur.o -pthread	
	
CLEAN: 
	rm -rf *.o 
