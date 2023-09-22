#include "enregistrement.h" 
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 

// ecrit dans un journal le mouvement d'une piece 
void ecrireMove(int x, int y, int newX, int newY, damier *d, int logFile){
	int ind = d->plateau[x][y]->index;
	int newInd = d->plateau[newX][newY]->index;
	char * log = malloc(sizeof(char)*256);
	sprintf(log, "%x%x", ind+128, newInd+128);
	write(logFile, log, sizeof(char)*strlen(log));
	log="";
}

// ecrit dans un journal les prises d'une piece
void ecrirePrise(pprise coup, damier *d, int logFile){
	char * log=malloc(sizeof(char)*256);
	while(coup!=NULL){
		char * str=malloc(sizeof(char)*16);
		if(coup->next==NULL){
			sprintf(str, "%x%x", d->plateau[coup->x][coup->y]->index, d->plateau[coup->newX][coup->newY]->index + 128);
		}
		else{
			sprintf(str, "%x", d->plateau[coup->x][coup->y]->index);
		}
		strcat(log, str);
		str = "";
		coup=coup->next;
	}
	write(logFile, log, sizeof(char)*strlen(log));
	log="";
}
