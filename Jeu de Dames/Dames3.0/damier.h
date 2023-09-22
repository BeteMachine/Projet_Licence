#ifndef DAMIER_H
#define DAMIER_H
#include "case.h"

typedef struct{
	Ccase * plateau[10][10];
	int nbNoir; //nombre de pièces noires restantes
	int nbBlanc;//nombre de pièces blanches restantes
	int coups; // nombre de coups (si il dépasse 100, la partie est finie)
}damier;
typedef struct prises{
	int x;
	int y;
	int newX;
	int newY;
	int nbPrises;
	struct prises * next;
}*pprise;

//initialise le damier de départ comme il se doit 
damier * damier_init();

//crée un damier à partir d'un damier
damier * damierFromDamier(damier *d);

// affiche le damier (cases blanches en 0, pions noirs en □ , pions blancs en ▪)
// Dames noires en N, Dames blanches en B
void affichage(damier * d);
void affichage_numero_case(damier *d);

//Libère toute la mémoire occupée par le damier. 
void damier_destroy(damier *d);
int checkLegalMove(int curX, int curY, int newX, int newY,damier * d, int couleur); // 0 false, 1 true 
int move(int curX, int curY, int newX, int newy,damier * d, int couleur);
int checkLegalEat(int curX, int curY, int newX, int newY,damier * d, int couleur); // 0 false, 1 true 
int eat(int curX, int curY, int newX, int newY, damier * d, int couleur);
int checkLegalMoveDame(int curX, int curY, int newX, int newY, damier *d , int couleur); // 0 false, 1 true 
int moveDame(int curX, int curY, int newX, int newY, damier *d, int couleur);
int checkEatDame(int curX, int curY, int newX, int newY, damier *d, int couleur);
int eatDame(int curX, int curY, int newX, int newY, damier *d , int couleur);
void transformToDame(piece * p);
int max(int *tab, int n);
pprise maxPriseCase(damier *d, int x, int y,int couleur);
pprise maxPriseCaseDame(damier *d, int x, int y, int couleur);
pprise maxPrise(damier *d, int couleur);
pprise maxPriseDame(damier *d, int couleur);
void prise_destroy(pprise p);
#endif 
