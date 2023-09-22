#include "piece.h"
#include <stdio.h> 
#include <stdlib.h> 

//initialise une piece de type t et de couleur c 
piece * piece_init(int t, int c){
	piece * res = malloc(sizeof(piece));
	res->type = t;
	res->couleur=c;
	return res;
}

//modifier le type de la piece , par exemple, un pion qui atteint le camp adverse devient une dame
void piece_setType(piece * p,int t){
	p->type=t;
}

//libérer l'espace occupé par une pièce 
void piece_destroy(piece * p){
	free(p);
	p=NULL;
}
