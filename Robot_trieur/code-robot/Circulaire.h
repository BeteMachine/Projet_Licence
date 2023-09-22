/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#ifndef __EX2__
#define __EX2__

#include "AlgoNaif.h"
#include "Grille.h"
#include "entree_sortie.h"
#include "Solution.h"
#include "API_AffGrille.h"

// Fonction qui recherche la case de couleur c le plus proche
// de la case (i,j) en méthode circulaire
void RechercheCaseCirculaire_c(Grille *G, int c, int i, int j, int *k, int *l);

// Fonction qui recherche la case non-noire contenant une pièce
// la plus proche de la case (i,j) en méthode circulaire
void RechercheCaseCirculaire_nn(Grille *G, int i, int j, int *k, int *l);

// Fonction qui implémente la version circulaire de l'algorithme au plus proche
void algorithme_circulaire(Grille *G, Solution *S);


#endif
