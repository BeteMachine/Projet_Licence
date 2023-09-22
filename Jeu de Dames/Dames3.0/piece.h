#ifndef PIECE_H
#define PIECE_H
#include <stdio.h>
#include <stdlib.h> 

typedef struct{
	int type; // 0 pour pion, 1 pour dame
	int couleur; //0 pour noir, 1 pour blanc
}piece;

//initialise une piece de type t et de couleur c 
piece * piece_init(int t, int c);

//modifier le type de la piece , par exemple, un pion qui atteint le camp adverse devient une dame
void piece_setType(piece * p, int t);

//libérer l'espace occupé par une pièce 
void piece_destroy(piece * p);

#endif
