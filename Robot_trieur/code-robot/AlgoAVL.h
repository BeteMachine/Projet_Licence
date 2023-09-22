/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#ifndef __EX4__
#define __EX4__

#include "AlgoNaif.h"
#include "Circulaire.h"
#include "Grille.h"
#include "entree_sortie.h"
#include "Solution.h"
#include "API_AffGrille.h"

typedef struct _node *AVL;

struct _node
{
    int key;                //valeur du noeud
    struct _node * left;    //fils gauche du noeud
    struct _node * right;   //fils droit noeud
    int h;                  //hauteur du noeud
    int eq;                 //equilibrage = hauteur(fils gauche) - hauteur(fils droit)
};

typedef struct AVL **M;

struct M
{
    int coul; //G->T[i][j].fond;
    int indice; //G->T[i];
    AVL ci; //arbre des cases de la ligne indice et de couleur coul (j)
};

/* Afficher les éléments dans l'ordre infixe */
void print_inorder_AVL (AVL A);

/* Recherche de v dans l'arbre binaire de recherche A */
int search_AVL (AVL A, int v);

/* Recherche du minimum dans un arbre binaire de recherche */
int minimum_AVL (AVL A);

/* Recherche du maximum */
int maximum_AVL (AVL A);

/* Mesure de la hauteur de l'arbre A */
int height_AVL (AVL A);

/* Effectue une rotation gauche de l'arbre A sur sa racine, attention au passage de A par reference */

void rot_G (AVL * A);

/* Effectue une rotation droite de l'arbre A sur sa racine, attention au passage de A par reference */
void rot_D (AVL * A);

/* fonction rotation (AVL *) qui contient les rotations faites lors de l'insertion et suppression */
void rotation (AVL * A);

/* Insertion de v dans l'arbre A , attention au passage de A par référence */
void insert_AVL (AVL* A, int v);

/* Recherche puis suppression de v dans l'arbre A, attention au passage de A par référence */
void delete_AVL (AVL* A, int v);

/* Liberation complete de la memoire occupee par un arbre */
void free_tree_AVL (AVL A);

/* Recherche la case la plus proche*/
int AVLrechercherPlusProcheCase(AVL *A, int i, int j);

/* algorithme */
void algo_AVL(Grille *G, Solution *S);

#endif
