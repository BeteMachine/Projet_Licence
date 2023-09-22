#ifndef __ENREGISTREMENT_H__ 
#define __ENREGISTREMENT_H__
#include "damier.h"
#include "envoi.h"

void ecrireMove(int x, int y, int newX, int newY, damier *d, int fd);
void ecrirePrise(pprise coup, damier *d, int fd);

#endif 

