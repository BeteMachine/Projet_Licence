#ifndef RANDOMPLAYER_H
#define RANDOMPLAYER_H
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h> 
#include "damier.h"
#include "exact_rw.h"
typedef struct{
	int couleur;
}randomPlayer;

randomPlayer * randomPlayer_init(int couleur);
void play(randomPlayer *rP, damier *d, int fd, int logfile);
void randomPlayerDestroy(randomPlayer * rP);

#endif

