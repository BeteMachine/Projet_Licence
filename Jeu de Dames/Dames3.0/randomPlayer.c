#include "randomPlayer.h"
#include "exact_rw.h"
#include "envoi.h"
#include "enregistrement.h"
#include <unistd.h> 
#include <time.h>


randomPlayer * randomPlayer_init(int couleur){
	randomPlayer * res = malloc(sizeof(randomPlayer));
	res->couleur = couleur;
	return res;
}

// bot qui joue aléatoirement
void play(randomPlayer *rP, damier *d, int fd, int logFile){	
	pprise ppr = malloc(sizeof(struct prises));
	pprise ppr2 =malloc(sizeof(struct prises));
	srand(time(NULL));
	while(1){
		int x = rand() % 10;
		int y = rand() % 10;
		int newX = rand() % 10;
		int newY = rand() % 10;
		ppr=maxPrise(d,rP->couleur);
		ppr2=maxPriseDame(d,rP->couleur);
		pprise pprcopy=maxPrise(d,rP->couleur);
		pprise ppr2copy=maxPriseDame(d,rP->couleur);
		// prises possible par une piece
		if((ppr!=NULL)&&((ppr2==NULL) ||((ppr->nbPrises)>(ppr2->nbPrises)))){
                	while (ppr!=NULL){
                		eat(ppr->x,ppr->y,ppr->newX,ppr->newY,d,rP->couleur);
                		printf("pièce du joueur %d, a effectué une prise de (%d,%d) à (%d,%d)\n",rP->couleur,ppr->x,ppr->y,ppr->newX, ppr->newY);
                		ppr=ppr->next;
                	}
                	envoyerPrise(d, pprcopy, fd);
                	ecrirePrise(pprcopy, d, logFile);
                	prise_destroy(ppr);
                	prise_destroy(pprcopy);
                	break;
                	
                }
                // prises possible par une dame
                else if((ppr2!=NULL)&&((ppr2->nbPrises>0)&&(ppr2->nbPrises<=25))){
                	while (ppr2!=NULL){
                		eatDame(ppr2->x,ppr2->y,ppr2->newX,ppr2->newY,d,rP->couleur);
                		printf("pièce du joueur %d, a effectué une prise de (%d,%d) à (%d,%d)\n",rP->couleur,ppr2->x,ppr2->y,ppr2->newX, ppr2->newY);
                		ppr2=ppr2->next;
                	}
                	
                	envoyerPrise(d, ppr2copy, fd);
                	ecrirePrise(ppr2copy, d, logFile);
                	prise_destroy(ppr2);
                	prise_destroy(ppr2copy);
                	break;
                }
                // mouvement d'une piece
		else if(move(x,y,newX,newY,d,rP->couleur)){
			envoyerMove(d,x,y,newX,newY,fd);
			ecrireMove(x, y, newX, newY, d, logFile);
			break;
		}
		// mouvement d'une dame
		else if(moveDame(x,y,newX,newY,d,rP->couleur)){
			envoyerMove(d,x,y,newX,newY,fd);
			ecrireMove(x, y, newX, newY, d, logFile);
			break;
		}
	}
}

void randomPlayerDestroy(randomPlayer * rP){
	free(rP);
	rP=NULL;
}
