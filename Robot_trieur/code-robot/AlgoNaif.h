/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#ifndef __EX1__
#define __EX1__

#include "Grille.h"
#include "entree_sortie.h"
#include "Solution.h"
#include "API_AffGrille.h"

// Fonction qui ajoute les caractères pour faire le chemin
// le plus court entre (i,j) et (k,l)
void PlusCourtChemin(Solution *S, int i, int j, int k, int l);

// Fonction qui recherche la case de couleur c le plus proche
// de la case (i,j)
void RechercheCaseNaif_c(Grille *G, int c, int i, int j, int *k, int *l);

// Fonction qui recherche la case non-noire contenant une pièce
// la plus proche de la case (i,j)
void RechercheCaseNaif_nn(Grille *G, int i, int j, int *k, int *l);

/* Fonction qui implémente la version naïve de l'algorithme au plus poroche
- Si le robot ne porte pas de piece, il va prendre la piece de la case la plus proche,
- Sinon le robot va placer sa piece sur la case de meme couleur que sa piece et qui est la plus
proche (en nombre de cases a parcourir).
S’il existe plusieurs cases a meme distance dans la grille, le robot choisit toujours la case “en haut, a
gauche”, c’est-a-dire la case de plus petit indice en ligne, et s’il y a plusieurs sur cette ligne celle de
plus petit indice en colonne.*/
void algorithme_naif(Grille *G, Solution *S);
// !!! Créer seulement le fichier Solution correspondant
// à tester vous même avec ./Checker_SortingRobot !!!

#endif
