/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#include "AlgoLDC.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

CelluleLDC* creerCellule(int i, int j)
{
  CelluleLDC *res = malloc(sizeof(CelluleLDC));
  res->i = i;
  res->j = j;
  res->prev = NULL;
  res->suiv = NULL;
  return res;
}

void LDCInitialise(LDC *ldc)
{
  ldc->premier = NULL;
  ldc->dernier = NULL;
}

int LDCVide(LDC *ldc)
{
  if(ldc->premier == NULL)
    return 1;
  return 0;
}

void LDCInsererEnFin(LDC *ldc, int i, int j)
{
  CelluleLDC *res = creerCellule(i, j);
  if(LDCVide(ldc))
  {
    ldc->premier = res;
    ldc->dernier = res;
  }
  else
  {
    ldc->dernier->suiv = res;
    res->prev = ldc->dernier;
    ldc->dernier = res;
  }
}

void LDCenleverCellule(LDC *ldc, CelluleLDC * cel)
{
  if (ldc->premier == cel)
  {
      if (ldc->premier == ldc->dernier)
      {
          free(ldc->premier);
          ldc = NULL;
          free(ldc);
          return;
      }
      ldc->premier = ldc->premier->suiv;
      if(ldc->premier != NULL){
         ldc->premier->prev = NULL;
     }
     free(ldc->premier->prev);
	 cel->suiv = NULL;
     return;
  }
  else
  {
       CelluleLDC *temp = ldc->premier;
       while((cel != temp) && (temp !=NULL))
       {
          temp = temp->suiv;
       }
       if(temp->suiv != NULL)
       {
          temp->prev->suiv = temp->suiv;
          temp->suiv->prev = temp->prev;
       }
       else
       {
          ldc->dernier->prev->suiv = NULL;
          ldc->dernier = ldc->dernier->prev;
       }
       free(temp);
    }
}

void LDCafficher(LDC *ldc)
{
  CelluleLDC *res;
  for(res=ldc->premier; res != NULL; res = res->suiv)
  {
      printf("(%d, %d)",res->i, res->j);
  }
  printf("\n");
}

void LDCdesallouer(LDC *ldc)
{
 while(!LDCVide(ldc))
  {
    LDCenleverCellule(ldc, ldc->premier);
  }
  free(ldc);
}

CelluleLDC* LDCrechercherPlusProcheCase(LDC* ldc, int i, int j)
{
    int res, res2;
    CelluleLDC *temp = malloc(sizeof(CelluleLDC));
    temp = ldc->premier;
    CelluleLDC *proche;
    res = abs(i - temp->i)+abs(j - temp->j);
    proche = temp;
    while(temp != NULL)
    {
        res2 = abs(i - temp->i)+abs(j - temp->j);
        if(res2 < res)
        {
            proche = temp;
            res = res2;
        }
        temp = temp->suiv;
    }
    return proche;
}

void algorithme_parcouleur(Grille *G, Solution *S)
{
    LDC *TC[G->nbcoul];
    int i, j, a, b, h, k, l;
    for(h = 0; h < G->nbcoul; h++)
    {
        TC[h] = malloc(sizeof(LDC));
        LDCInitialise(TC[h]);
    }
    for(i = 0; i < G->m; i++)
    {
        for(j = G->n - 1; j >= 0; j--)
        {
            if(G->T[i][j].fond != G->T[i][j].piece)
                LDCInsererEnFin(TC[G->T[i][j].fond], i, j);
        }
    }
    while((G->cptr_noire != G->n * G->m))
    {
        if(G->T[G->ir][G->jr].robot == -1)
        {
            RechercheCaseCirculaire_nn(G, G->ir, G->jr, &k, &l);
            PlusCourtChemin(S, G->ir, G->jr, k, l);
        }
        if(G->T[G->ir][G->jr].robot > -1)
        {
            CelluleLDC *res = LDCrechercherPlusProcheCase(TC[G->T[G->ir][G->jr].robot], G->ir, G->jr);
            PlusCourtChemin(S, G->ir, G->jr, res->i, res->j);
            k = res->i;
            l = res->j;
            LDCenleverCellule(TC[G->T[res->i][res->j].fond], res);
        }
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
Nous avons pu remarquer cette fonction est beaucoup plus rapide que les precedentes.
Il semblerait que plus le nombre de couleur est bas et plus l'algorithme prendra
du temps, à contrario plus son nombre sera elevé plus il sera rapide.
Nous avons remarquer que parfois nous ne puissions pas faire plus de 100 millions de pas,
majoritairement le processus s'arrête de lui même si c'est le cas.
Ainsi la vitesse est grandement influencer par le nombre de couleur, nous avons testé
pour une grille de 620 620 14 500 et avons eu 29.5 secondes.
Avec un plus grand plus grand nombre de couleur l'algorithme prendra moins de temps
à se finir.


Calcule de la compléxité dans le pire cas n = m:

*La compléxité de la fonction PlusCourtChemin:

T(n) = c1 + n + n + c2
T(n) = 2n + c3 = O(n)

*La compléxité de la fonction RechercheCaseNaif_nn:

T(n) = c1 + n *(n + c2 +c3)
T(n) = c1 + n² = O(n²)

*La compléxité de la fonction LDCrechercherPlusProcheCase:
Dans le pire cas on a la moitié de la grille qui soit de même couleur ayant une pièce
de couleur différente (pire des cas) soit :

T(n) = c1 + n²/2 = O(n²/2)

*La compléxité de la fonction LDCInsererEnFin:

T(n) = c1 = O(1)

*La compléxité de la fonction algorithme_parcouleur:

T(n) = n + n *(n + c0) + n² *(c1 + O(n²) + O(n) + n + O(n²/2) + O(n) + n + c2)
T(n) = n + n² + c0n + n² * (2n² + 4n + c3)
T(n) = n⁴ + 4n³ + c3n² + n² c0n + n  = O(n⁴)

Soit α le nombre de maximal de pièce d'une même couleur dans la grille
α = n²/2

T(n) = n + n *(n + c0) + n² *(c1 + O(n²) + O(n) + n + O(n²/2) + O(n) + n + c2)

T(n) = O(n⁴)

*/
