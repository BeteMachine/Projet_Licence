#ifndef __ADVERSE_H__
#define __ADVERSE_H__
#include "damier.h" 
pprise convertToPrise(char * coup, damier *da);
int * convertToMove(char * coup, damier *da);
int verificationPriseAdverse(damier *d, char * coup, int couleur);
int verificationMoveAdverse(damier *d, char * coup, int couleur);
int jouerCoupAdverse(damier *d, char * coup, int couleur);
#endif

