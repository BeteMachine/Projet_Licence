/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#include "Circulaire.h"
#include "AlgoNaif.h"
#include "Solution.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void RechercheCaseCirculaire_c(Grille *G, int c, int i, int j, int *k, int *l){
    int f = 0;
    int lg, ld;
    int g = 1;
    int trouve = 0;
    while((trouve != 1)&&(f>-1))
    {
        f = i - g;
        lg = j;
        ld = j;
        while((trouve != 1)&&(f <= i)){
            if((trouve != 1)&&(f >= 0) && (f < G->m) && (lg >=0) && (lg < G->n))
            {
                if((G->T[f][lg].fond == c)&&(G->T[f][lg].piece != c))
                {
                    *k = f;
                    *l = lg;
                    trouve = 1;
                }
            }
            if((trouve != 1)&&(f >= 0) && (f < G->m) && (ld >=0) && (ld < G->n))
            {
                if((G->T[f][ld].fond == c)&&(G->T[f][ld].piece != c))
                {
                    *k = f;
                    *l = ld;
                    trouve = 1;
                }
            }
            f += 1;
            lg -= 1;
            ld += 1;
        }
        f = i + 1;
        lg = j - g + 1;
        ld = j + g - 1;
        while((trouve != 1)&&(f <= i + g))
        {
            if((trouve != 1)&&(f >= 0) && (f < G->m) && (lg >=0) && (lg < G->n))
            {
                if((G->T[f][lg].fond == c)&&(G->T[f][lg].piece != c))
                {
                    *k = f;
                    *l = lg;
                    trouve = 1;
                }
            }
            if((trouve != 1)&&(f >= 0) && (f < G->m) && (ld >=0) && (ld < G->n))
            {
                if((G->T[f][ld].fond == c)&&(G->T[f][ld].piece != c))
                {
                    *k = f;
                    *l = ld;
                    trouve = 1;
                }
            }
            f += 1;
            lg += 1;
            ld -= 1;
        }
        g++;
    }
    return;
}

void RechercheCaseCirculaire_nn(Grille *G, int i, int j, int *k, int *l){
    int f = 0;
    int lg, ld;
    int a, b;
    int g = 1;
    int trouve = 0;
    if((G->T[i][j].fond != G->T[i][j].piece) && (G->T[i][j].piece != -1))
    {
        *k = i;
        *l = j;
        trouve = 1;
        return;
    }
    while((trouve != 1)&&(f>-1))
    {
        f = i - g;
        lg = j;
        ld = j;
        while((trouve != 1)&&(f <= i))
        {
            if((trouve != 1)&&(f >= 0) && (f < G->m) && (lg >=0) && (lg < G->n))
            {
                if((G->T[f][lg].fond != G->T[f][lg].piece) && (G->T[f][lg].piece != -1))
                {
                    *k = f;
                    *l = lg;
                    trouve = 1;
                }
            }
            if((trouve != 1)&&(f >= 0) && (f < G->m) && (ld >=0) && (ld < G->n))
            {
                if((G->T[f][ld].fond != G->T[f][ld].piece) && (G->T[f][ld].piece != -1))
                {
                    *k = f;
                    *l = ld;
                    trouve = 1;
                }
            }
            f += 1;
            lg -= 1;
            ld += 1;
        }
        f = i + 1;
        lg = j - g + 1;
        ld = j + g - 1;
        while((trouve != 1)&&(f <= i + g))
        {
            if((trouve != 1)&&(f >= 0) && (f < G->m) && (lg >=0) && (lg < G->n))
            {
                if((G->T[f][lg].fond != G->T[f][lg].piece) && (G->T[f][lg].piece != -1))
                {
                    *k = f;
                    *l = lg;
                    trouve = 1;

                }
            }
            if((trouve != 1)&&(f >= 0) && (f < G->m) && (ld >=0) && (ld < G->n))
            {
                if((G->T[f][ld].fond != G->T[f][ld].piece) && (G->T[f][ld].piece != -1))
                {
                    *k = f;
                    *l = ld;
                    trouve = 1;
                }
            }
            f += 1;
            lg += 1;
            ld -= 1;
        }
        g++;
    }
    return;
}

void algorithme_circulaire(Grille *G, Solution *S){
    int k, l;
	while((G->cptr_noire != G->n * G->m))
	{
		if(G->T[G->ir][G->jr].robot == -1){
			RechercheCaseNaif_nn(G, G->ir, G->jr, &k, &l);
			PlusCourtChemin(S, G->ir, G->jr, k, l);
        }
        if(G->T[G->ir][G->jr].robot > -1){
    		RechercheCaseCirculaire_c(G, G->T[G->ir][G->jr].robot, G->ir, G->jr, &k, &l);
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

/*
nous calculons la compléxité au pire cas quand n = m
La compléxité de la fonction RechercheCaseCirculaire_c:

T(n) = c1 + n * (n + c2 + n + c3) + c4
T(n) = n * (2n + c2+c3) + c1+c4
T(n) = 2n^2 + c5n + c6 = O(n^2)

La compléxité de la fonction RechercheCaseCirculaire_nn:

T(n) = c1 + n * (n + c2 + n + c3) + c4
T(n) = n * (2n + c2+c3) + c1+c4
T(n) = 2n^2 + c5n + c6 = O(n^2)

La compléxité de la fonction PlusCourtChemin:

T(n) = c1 + n + n + c2
T(n) = 2n + c3 = O(n)

La compléxité de la fonction algorithme_circulaire
se calcule avec les compléxité de RechercheCaseCirculaire_c
, de RechercheCaseCirculaire_nn ainsi que de PlusCourtChemin
donc on a:

T(n) = n^2 * (c1 + O(n^2) + O(n) + n + O(n^2) + O(n) + n + c2)
T(n) = n^2 * (2n^2 + 4n + c3)
T(n) = n^4 + 4n^3 + c3n^2 = O(n^4)

Remarque :
Nous avons rencontré un problème lier au nombre de lignes et de colonnes.
En effet lorsque l'on entre une grille de taille 255*255 de nombre de couleur 21 et de
graine 10, la machine ne donne pas de résultat même après plus de 6 minutes d'attente.
On a tester avec d'autre graine et cela fonctionnait en environ 8 secondes.
Lorsqu'on y entre une taille supérieur à 255*255 la machine nous renvoie une erreur de ségmentation.
(et ce peut importe la graine ou le nombre de couleur supérieur à 2)
PS:
Nous avons résolu le problème,
en fait on avait mit l'affichage dans le main qui ne pouvait pas afficher
une grille de telle taille ce qui nous valait une erreur de segmentation.
*/
