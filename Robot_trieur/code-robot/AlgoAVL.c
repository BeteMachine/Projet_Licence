/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#include "AlgoAVL.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_inorder_AVL(AVL A)
{
   if (A != NULL)
   {
      print_inorder_AVL(A->left);
      printf("[%d, h:%d, eq:%d] ",A->key,A->h,A->eq);
      print_inorder_AVL(A->right);
   }
}

int search_AVL(AVL A, int v)
{
   if(A == NULL)
      return 0;
   if(A->key == v)
      return 1;
   if(v < A->key)
      return search_AVL (A->left,v);
   else
      return search_AVL (A->right,v);
}

int minimum_AVL (AVL A)
{
   if(A == NULL)
      return -1;
   if(A->left == NULL)
      return A->key;
   return minimum_AVL(A->left);
}

int maximum_AVL(AVL A)
{
   if(A == NULL)
      return -1;
   if(A->right == NULL)
      return A->key;
   return maximum_AVL(A->right);
}

int height_AVL(AVL A)
{
   if (A == NULL)
      return -1;
   return A->h;
}

void rot_G(AVL* A)
{
	AVL tmp;
	if(((*A) != NULL) && ((*A)->right != NULL))
   {
	  //on modifie les pointeurs pour effectuer la rotation
      tmp = (*A)->right;
      (*A)->right = tmp->left;
      tmp->left = (*A);
      (*A) = tmp;
      //on recalcule l'equilibrage de (*A)->left, l'ancienne racine
      if (height_AVL((*A)->left->left) > height_AVL((*A)->left->right))
         (*A)->left->h = 1 + height_AVL((*A)->left->left);
      else
         (*A)->left->h = 1 + height_AVL((*A)->left->right);
      (*A)->left->eq = height_AVL((*A)->left->left) - height_AVL((*A)->left->right);
      //on recalcule l'equilibrage de (*A), l'ancien fils droit
      if (height_AVL((*A)->left) > height_AVL((*A)->right))
         (*A)->h = 1 + height_AVL((*A)->left);
      else
         (*A)->h = 1 + height_AVL((*A)->right);
      (*A)->eq = height_AVL((*A)->left) - height_AVL((*A)->right);
    }
}

void rot_D(AVL* A)
{
	AVL tmp;
	if (((*A) != NULL) && ((*A)->left != NULL))
   {
	  //on modifie les pointeurs pour effectuer la rotation
      tmp = (*A)->left;
      (*A)->left = tmp->right;
      tmp->right = (*A);
      (*A) = tmp;
      //on recalcule l'equilibrage de (*A)->right, l'ancienne racine
      if (height_AVL((*A)->right->left) > height_AVL((*A)->right->right))
        (*A)->right->h = 1 + height_AVL((*A)->right->left);
      else
         (*A)->right->h = 1 + height_AVL((*A)->right->right);
      (*A)->right->eq = height_AVL((*A)->right->left) - height_AVL((*A)->right->right);
      //on recalcule l'equilibrage de (*A), l'ancien fils gauche
      if (height_AVL((*A)->left) > height_AVL((*A)->right))
        (*A)->h = 1 + height_AVL((*A)->left);
      else
        (*A)->h = 1 + height_AVL((*A)->right);
      (*A)->eq = height_AVL((*A)->left) - height_AVL((*A)->right);
    }
}

void rotation(AVL * A)
{
	if ((*A) != NULL)
   {
      if (height_AVL((*A)->left) > height_AVL((*A)->right))
         (*A)->h = 1 + height_AVL((*A)->left);
      else
        (*A)->h = 1 + height_AVL((*A)->right);
      (*A)->eq = height_AVL((*A)->left) - height_AVL((*A)->right);
      if ((*A)->eq == 2) //l'arbre est trop haut à gauche
      {
         if (height_AVL((*A)->left->right) > height_AVL((*A)->left->left)) //l'arbre "du milieu" est le plus gros -> double rotation
            rot_G(&((*A)->left));
         //dans tous les cas on finit par une rotation à droite
         rot_D(A);
      }
      else
      {
         if((*A)->eq == -2) //l'arbre est trop haut a droite
         {
           if(height_AVL((*A)->right->left) > height_AVL((*A)->right->right))
               rot_D(&((*A)->right));
           rot_G(A);
         }
      }
   }
}

void insert_AVL(AVL* A, int v)
{
   if ((*A) == NULL)
   {
	   (*A) = malloc(sizeof(struct _node));
	   (*A)->key = v;
	   (*A)->left = NULL;
   	(*A)->right = NULL;
	   (*A)->h = 0;
	   (*A)->eq = 0;
   }
   else
   {
	   //On insère à la position definie par la propriété d'ABR */
      if (v < (*A)->key)
         insert_AVL(&((*A)->left),v);
      else
         insert_AVL(&((*A)->right),v);
      //on met à jour l'hauteur et l'équlibrage
      rotation(A);
   }
}

void delete_AVL(AVL* A, int v)
{
   if ((*A) != NULL)
   {
	   AVL T;
	   //on supprime l'élément v
      if ((*A)->key == v)
      {
	      if ((*A)->left == NULL)
	      {
		      T = (*A);
		      (*A) = (*A)->right;
		      free(T);
	      }
	      else if ((*A)->right == NULL)
	      {
		      T = (*A);
		      (*A) = (*A)->left;
		      free(T);
	      }
	      else
	      {
	         int maxgauche = maximum_AVL((*A)->left);
	         (*A)->key = maxgauche;
	         delete_AVL(&((*A)->left),maxgauche);
         }
      }
      else
      {
         if (v < (*A)->key)
            delete_AVL(&((*A)->left),v);
         else
            delete_AVL(&((*A)->right),v);
      }
     //on met à jour l'hauteur et l'équlibrage
      rotation(A);
   }
}

void free_tree_AVL(AVL A)
{
   if (A != NULL)
   {
      free_tree_AVL(A->left);
      free_tree_AVL(A->right);
      free(A);
   }
}

/*int AVLrechercherPlusProcheCase(AVL *A, int i, int j)
{
    int res;
    if((*A) == NULL)
        return -1;
    res = abs((*A)->key - j);
    if(j < (*A)->key){
        res2 = AVLrechercherPlusProcheCase((*A)->left, i, j);
        if(res2 < res && res2 != -1)
            res = res2;
    }
    if(j > (*A)->key){
        res2 = AVLrechercherPlusProcheCase((*A)->right, i, j);
        if(res2 < res && res2 != -1)
            res = res2;
    }
    return res;
}

void algo_AVL(Grille *G, Solution *S)
{
    int i, j, k, l, a, b, h;
    for(i = 0; i < G->; i++)


    while((G->cptr_noire != G->n * G->m))
    {
        if(G->T[G->ir][G->jr].robot == -1)
        {
            RechercheCaseCirculaire_nn(G, G->ir, G->jr, &k, &l);
            PlusCourtChemin(S, G->ir, G->jr, k, l);
        }
        if(G->T[G->ir][G->jr].robot > -1)
        {
            int res = AVLrechercherPlusProcheCase(M[G->T[res->i][res->i].robot][G->ir], G->ir, G->jr);
            if(res == -1)
            ;
            PlusCourtChemin(S, G->ir, G->jr, M->indice, res);
            k = i;
            l = res;
            delete_AVL(M[G->T[res->i][res->i].fond][G->ir], res);
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
    free_tree_AVL(A);
}*/

/* Nous n'avons malheureusement pas réussi à finaliser notre projet sur l'exercice des AVL,
   causé par un manque de compréhension de notre part sur le fonctionnement de la recherche et la structure.
   Nous avons certaines idées que n'arrivons pas à retransmettre à l'écrit.
   Nos fonctions AVL fonctionnent bien avec notre test sans nos 2 dernières fonctions.

   Pour ce qui est de la compléxité de nos fonctions AVL étant donné que ce sont des AVL leurs compléxités est donc de O(log(n)).
   Pour Alogritme_AVL la complexite serait de O(n^3) car dans le pire des cas on cherche depuis la premiere ligne la case
   la plus proche qui se situe a la derniere ligne ce qui nous fait une complexite O(n) et etant la fonction
   la plus couteuse en temps et ce dans la boucle while de taille n*m d'ou le O(n^3)).
*/
