/* CHEN kevin
 * MARICIC Alexandre
 * TP5
 */
#include <time.h>
#include "AlgoAVL.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char **argv)
{
    Grille G;
    AffGrille AG;
    Solution S;
    int graine;
    int i, j;
    long cl = CLOCKS_PER_SEC;
    clock_t t1, t2;
    if (argc != 5)
    {
        printf ("usage: %s <nb_lignes nb_colonnes nb_couleur graine>\n", argv[0]);
        return 1;
    }

    G.m = atoi (argv[1]);
    G.n = atoi (argv[2]);
    G.nbcoul = atoi (argv[3]);
    if (G.nbcoul > G.m * G.n)
    {
        printf ("Il ne doit pas y avoir plus de couleurs que de cases.\n");
        exit (1);
    }
    graine = atoi (argv[4]);

    Grille_allocation (&G);

    Gene_Grille (&G, graine);

    Solution_init (&S);

    t1 = clock();
    //algo_AVL(&G, &S);
    t2 = clock();
    printf("AlgoAVL :Temps consomme : %lf sec",(double)(t2-t1)/(double)cl);
    printf ("\nSolution valide en %d pas.\n", S.cptr_pas);

    //Ecriture_Disque (G.m, G.n, G.nbcoul, graine, &S);

return 0;
}
