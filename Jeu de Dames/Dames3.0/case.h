#ifndef CASE_H
#define CASE_H
#include "piece.h"

typedef struct{
	piece * p; //NULL si y a pas de pièce 
	int fond; //0 pour noir, 1 pour blanc
	int index; // -1 pour les blanches, de 1 à 50 pour les noires
}Ccase;

//initialise une case de fond f mais sans pièce
Ccase * case_init_vide(int f, int i);

//initialise une case de fond f, avec une piece de type t et de couleur c 
Ccase * case_init(int t, int c, int f, int i);

//renvoie la piece après l'avoir retiré de la case
piece * retirer_piece(Ccase * ca);

//creer une piece passer en argument à une case vide
void modifier_piece(piece * pi,Ccase *ca);

//detruit une case
void case_destroy(Ccase * ca);

#endif

