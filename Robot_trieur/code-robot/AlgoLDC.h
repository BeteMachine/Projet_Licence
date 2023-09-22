/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#ifndef __EX3__
#define __EX3__

#include "AlgoNaif.h"
#include "Circulaire.h"
#include "Grille.h"
#include "entree_sortie.h"
#include "Solution.h"
#include "API_AffGrille.h"

typedef struct celluleLDC{
    int i, j;
    struct celluleLDC *prev;  /* pointeur sur l’element precedent de la liste */
    struct celluleLDC *suiv ; /* pointeur sur l’element suivant de la liste */
}CelluleLDC ;

typedef struct{
    CelluleLDC* premier;
    CelluleLDC* dernier;
}LDC;

// alloue et retourne une cellule
CelluleLDC* creerCellule(int i, int j);

// initialise une liste
void LDCInitialise(LDC *ldc);

// teste si la liste est vide
int LDCVide(LDC * ldc);

// insere une nouvelle cellule en fin
void LDCInsererEnFin(LDC * ldc , int i , int j);

// supprime une cellule a partir d un pointeur sur la cellule
void LDCenleverCellule(LDC * ldc , CelluleLDC * cel);

// un affichage en cas de besoin pour debugage
void LDCafficher(LDC * ldc);

// desalloue toute la liste (si elle n est pas vide a la fin)
void LDCdesallouer(LDC * ldc);

// recherche dans une liste la premiere case la plus proche de (i, j)
CelluleLDC* LDCrechercherPlusProcheCase(LDC* ldc, int i, int j);

// alloue et intialise le tableau TC et le remplit en parcourant la grille de haut en bas et de droite à gauche
void algorithme_parcouleur(Grille *G, Solution *S);
#endif
