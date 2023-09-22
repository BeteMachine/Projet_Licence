#include "case.h"
#include <stdio.h>
#include <stdlib.h> 

//initialise une case de fond f mais sans pièce
Ccase * case_init_vide(int f, int i){
	Ccase * res = malloc(sizeof(Ccase));
	res->p=NULL;
	res->fond =f;
	res->index=i;
	return res;
}

//initialise une case de fond f, avec une piece de type t et de couleur c 
Ccase * case_init(int t, int c, int f, int i){
	Ccase * res = malloc(sizeof(Ccase));
	res->p = piece_init(t,c);
	res->fond=f;
	res->index=i;
	return res;
}

//renvoie la piece après l'avoir retiré de la case
piece * retirer_piece(Ccase * ca){
	piece * res = piece_init(ca->p->type,ca->p->couleur);
	ca->p=NULL;
	return res;
}

//creer une piece passer en argument à une case vide
void modifier_piece(piece * pi,Ccase *ca){
	if(ca->p==NULL){
		ca->p=pi;
	}
}

//detruit une case
void case_destroy(Ccase * ca){
	piece_destroy(ca->p);
	free(ca);
	ca=NULL;
}
