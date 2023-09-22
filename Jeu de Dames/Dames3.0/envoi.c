#include "envoi.h"
#include "exact_rw.h"
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 


// ecrit le mouvement à partir d'une position de départ et d'arrivé dans un tableau de char pour l'envoyer à l'adversaire
char * convertMoveTochar(int x, int y, int newX, int newY, damier *d){  
	int ind = d->plateau[x][y]->index;
	int newInd = d->plateau[newX][newY]->index;
	char * move = malloc(sizeof(char)*5);
	sprintf(move, "%d-%d", ind, newInd);
	return move;
}

// ecrit les prises à partir d'une structure "prise" dans un tableau de char pour l'envoyer à l'adversaire
char * convertPriseTochar(pprise p, damier *d){
	char * prise = malloc(sizeof(char)*256);
	while(p!=NULL){
		char * buff = malloc(sizeof(char)*6);
		if(p->next==NULL){
			 sprintf(buff, "%dx%d", d->plateau[p->x][p->y]->index, d->plateau[p->newX][p->newY]->index);
	        }
	        else{
	        	sprintf(buff, "%dx", d->plateau[p->x][p->y]->index);
	        }
	        strcat(prise, buff);
	        free(buff);
	        p=p->next;
	}
	printf("prise : %s \n", prise);
	sleep(1);
	return prise;
}

// envoie le mouvement d'une piece à l'adversaire
void envoyerMove(damier *d,int x, int y, int newX, int newY, int fd){
	char * move = convertMoveTochar(x, y, newX, newY, d);
	move[strlen(move)]='\0';
	printf("move : %s \n", move);
	sleep(1);
	exact_write(fd, move, sizeof(char)*(strlen(move)+1));
	move = "";
}

// envoie les prises d'une piece à l'adversaire
void envoyerPrise(damier *d, pprise p, int fd){
	char * prise = convertPriseTochar(p, d);
	prise[strlen(prise)]='\0';
	exact_write(fd, prise, sizeof(char)*(strlen(prise)+1));
	prise = "";
}
