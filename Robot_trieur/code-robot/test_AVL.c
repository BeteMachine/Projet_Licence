/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#include "AlgoAVL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
  AVL A = NULL;
  
  insert_AVL(&A,21);
  insert_AVL(&A,36);
  
  printf("\nInsertion de 21 et de 36 :\n");
  print_inorder_AVL(A);
  printf("\n");
    
  insert_AVL(&A,2);
  
  printf("\nInsertion de 2 :\n");
  print_inorder_AVL(A);
  printf("\n");
  
  insert_AVL(&A,10);
  insert_AVL(&A,11);  
  insert_AVL(&A,12);
  
  printf("\nInsertion de 10, 11 et 12:\n");
  print_inorder_AVL(A);
  printf("\n");  
  
  delete_AVL(&A,36);
  
  printf("\nSuppression de 36:\n");
  print_inorder_AVL(A);
  printf("\n"); 
    
  delete_AVL(&A,2);
  
  printf("\nSuppression de 2 :\n");
  print_inorder_AVL(A);
  printf("\n"); 
  
  delete_AVL(&A,12);
  
  printf("\nSuppression de 12 :\n");
  print_inorder_AVL(A);
  printf("\n");   
  
  free_tree_AVL(A);
  
  return 1;
}


