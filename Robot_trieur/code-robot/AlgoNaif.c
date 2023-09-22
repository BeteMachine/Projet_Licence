/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#include "AlgoNaif.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void PlusCourtChemin(Solution *S, int i, int j, int k, int l){
	for(;i > k;i--)
		Ajout_action(S, 'U');
	for(;j > l; j--)
		Ajout_action(S, 'L');
	for(;i < k; i++)
		Ajout_action(S, 'D');
	for(;j < l; j++)
		Ajout_action(S, 'R');
}

void RechercheCaseNaif_c(Grille *G, int c, int i, int j, int *k, int *l){
	int a, b;
	int t = 10000;
	int p = 10000;
	for(a = 0; a < G->m; a++){
		for(b = 0; b < G->n; b++){
			if((G->T[a][b].fond == c)&&(G->T[a][b].piece != c)){
				//Vérifie le nombre de pas à effectuer
				//si plus petit on change sinon on ne fait rien
				if((abs(i-a)+abs(j-b)) < (abs(i-t)+abs(j-p)))
				{
					*k = a;
					*l = b;
					//Sauvegarde de la position
					t = a;
					p = b;
				}
			}
		}
	}
}


void RechercheCaseNaif_nn(Grille *G, int i, int j, int *k, int *l){
	int a, b;
	int t = 10000;
	int p = 10000;
	if((G->T[i][j].fond != G->T[i][j].piece) && (G->T[i][j].piece != -1)){
		*k = i;
		*l = j;
		return;
	}
	for(a = 0; a < G->m; a++){
		for(b = 0; b < G->n; b++){
			if((G->T[a][b].fond != G->T[a][b].piece) && (G->T[a][b].piece != -1)){
				//Vérifie le nombre de pas à effectuer
				//si plus petit on change sinon on ne fait rien
				if((abs(i-a)+abs(j-b)) < (abs(i-t)+abs(j-p))){
					*k = a;
					*l = b;
					//Sauvegarde de la position
					t = a;
					p = b;
				}
			}
		}
	}
}

void algorithme_naif(Grille *G, Solution *S){
	int k, l;
	while((G->cptr_noire != G->n * G->m))
	{
		if(G->T[G->ir][G->jr].robot == -1){
			RechercheCaseNaif_nn(G, G->ir, G->jr, &k, &l);
			PlusCourtChemin(S, G->ir, G->jr, k, l);
		}
		if(G->T[G->ir][G->jr].robot > -1){
			RechercheCaseNaif_c(G, G->T[G->ir][G->jr].robot, G->ir, G->jr, &k, &l);
			PlusCourtChemin(S, G->ir, G->jr, k, l);
		}
		int a = G->ir;
		int b = G->jr;
		for(;a>k;){
			a--;
			changement_case(G, a, b);
		}
		for(;b>l;){
			b--;
			changement_case(G, a, b);
		}
		for(;a<k;){
			a++;
			changement_case(G, a, b);
		}
		for(;b<l;){
			b++;
			changement_case(G, a, b);
		}
		Ajout_action (S, 'S');
		swap_case(G);
	}
}

//Q1.1
/* 	Cas de base : |k-i| = |k - 0|
 *		|k-i| = k cases verticales pour (k, j)
 *	Hypothese de recurrence :
 *		On suppose que ce soit vraie pour |k-i| = k-i
 *		soit
 *			|k+1-i| = |k-i|+1
 *			Comme |k-i| est le nombre de cases verticales pour(k-i, j)
 *			Alors |k-i|+1 est le nombre de cases verticales pour (k-i+1, j)
 *	Cas de base : |l-j| = |l - 0|
 *			|l-j| = l cases horizontales pour (i, l)
 *		Hypothese de recurrence :
 *			On suppose que ce soit vraie pour |l-j| = l-j
 *			soit
 *				|l+1-j| = |l-i|+1
 *				Comme |l-j| est le nombre de cases horizontales pour(i, l-j)
 *				Alors |l-j|+1 est le nombre de cases horizontales pour (i, l+1-j)
 *	Et inversement pour les autres cas (similaires)
 */

//Q1.3
//	G->T[i][j] désigne la case a la ligne i et colonne j
//	G->T[i][j].fond = int (couleur de fond)
//	G->T[i][j].fond == G->T[i][j].piece alors la case est noire
//	G->T[i][j].piece = -1 alors piece est noire donc pas de pièce sur la case
//  G->T[i][j].robot = -1 alors le robot est présent sur la case mais sans pièce
//  G->T[i][j].robot entre 0 et nbcoul-1 pour la couleur de la pièce tenue par le robot

//Q1.6
//	L'algorithme a du mal a terminer les grilles d'une grandeurs de 16x16 en 30sec
//	pour la plupart du temps et au-dessus il n'y arrive pas
