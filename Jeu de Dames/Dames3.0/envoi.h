#ifndef __ENVOI_H__
#define __ENVOI_H__ 
#include "damier.h" 
#include "exact_rw.h"
char * convertMoveTochar(int x, int y, int newX, int newY, damier *d);
char * convertPriseTochar(pprise p, damier *d);
void envoyerMove(damier *d,int x, int y, int newX, int newY, int fd);
void envoyerPrise(damier *d, pprise p, int fd);

#endif
