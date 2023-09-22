#include "adverse.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

// convertit un tableau de caractere en prise
pprise convertToPrise(char * coup,damier *da){
    int len = strlen(coup);
    int compteur = 0;
    
    //compte le nombre de prises
    for(int i = 0 ;i < len;i++){
        if(coup[i] == 'x'){
            compteur++;
        }
    }

    // si pas de prise, on sort
    if(strchr(coup, 'x')==NULL){
        return NULL;
    }
    pprise res = malloc(sizeof(struct prises));
    pprise cur = res;
    char * c = malloc(sizeof(char)*len);
    
    // enregistre dans un tableau chaque coup
    for(int i=0; i<len;i++){
    	c[i]=coup[i];
    }
    char d[]="x";
    char * p =strtok(c, d);
    int *tab = malloc(sizeof(int)*(compteur+1));
    int a = 0;
    
    //supprime les 'x' dans le tableau
    while(p!=NULL){
        tab[a] = atoi(p);
        a++;
        p=strtok(NULL,d);
    }
    int b = 0;
    
    //convertit chaque prise en coordonnées x et y
    while(b+1 < compteur+1)    
    {
    	// recherche l'index correspondant à la prise à jouer
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                if(da->plateau[i][j]->index==tab[b]){
                    cur->x=i;
                    cur->y=j;
                }
                else if(da->plateau[i][j]->index==tab[b+1]){
                    cur->newX=i;
                    cur->newY=j;
                }
            }
        }
        cur->nbPrises=compteur - b;
        if(b!=compteur-1){
		cur->next=malloc(sizeof(struct prises));
		cur=cur->next;
	}
	else{
		cur->next=NULL;
		break;
	}
        b++;
    }
    
    return res;
}

// convertit un tableau de caractere en mouvement
int * convertToMove(char * coup, damier * da){
    int len = strlen(coup);
    int compteur = 0;
    for(int i = 0 ;i < len;i++){
        if(coup[i] == '-'){
            compteur++;
        }
    }


    if(strchr(coup, '-')==NULL){
        return NULL;
    }
    char * cur = malloc(sizeof(char)*strlen(coup));
    for(int i=0; i<strlen(coup);i++){
    	cur[i]=coup[i];
    }
    int * coordonnees = malloc(sizeof(int)*4);
    char d[]="-";
    char * p =strtok(cur, d);
    int *tab = malloc(sizeof(int)*(compteur+1));
    int a = 0;
    while(p!=NULL){
        tab[a] = atoi(p);
        a++;
        p=strtok(NULL,d);
    }
    
    // recherche des cases correspondant au mouvement
    for(int i=0;i<10;i++){
       for(int j=0;j<10;j++){
            if(da->plateau[i][j]->index==tab[0]){
                 coordonnees[0]=i;
                 coordonnees[1]=j;
            }
            else if(da->plateau[i][j]->index==tab[1]){
                 coordonnees[2]=i;
                 coordonnees[3]=j;
            }
        }
   }
   return coordonnees;
}

// verifie si une prise de l'adversaire est valide et retourne 1 si c'est valide, sinon 0
int verificationPriseAdverse(damier *d, char * coup, int couleur){
    if(strchr(coup, 'x')==NULL){
    	return 0;
    }
    pprise ppr=maxPrise(d, couleur);
    pprise ppr2=maxPriseDame(d, couleur);
    pprise prise_adverse=convertToPrise(coup,d);
    
    //prise avec la dame possible
    if(ppr2!=NULL){
    	if((ppr2->nbPrises <=0)||(ppr2->nbPrises >25)){
    		ppr2=NULL;
    	}
    }
    
    //prise avec une piece possible
    if(ppr!=NULL){
    	
    	//si il n'y a pas de prise avec la dame ou le nombre de prise avec une piece est supérieur à la dame
    	if((ppr2==NULL)||(ppr->nbPrises > ppr2->nbPrises)){
	    	if(prise_adverse->nbPrises != ppr->nbPrises){
	    		return 0;
	    	}
	    	damier *d2 = damierFromDamier(d);
	    	
	    	//tant que l'adversaire peut faire une prise il fait la prise et recommence
	    	while(prise_adverse!=NULL){
	    		if(eat(prise_adverse->x, prise_adverse->y, prise_adverse->newX, prise_adverse->newY, d2, couleur)){
	    			prise_adverse=prise_adverse->next;
	    		}
	    		else{
	    			return 0;
	    		}
	    	}
	    	return 1;
	}
	
	//si il n'y a pas de prise avec une piece mais une prise avec une dame
	else{ 
		if(prise_adverse->nbPrises != ppr2->nbPrises){
	    		return 0;
	    	}
	    	damier *d2 = damierFromDamier(d);
	    	
	    	//tant que la dame peux manger, elle mange
	    	while(prise_adverse!=NULL){
	    		if(eatDame(prise_adverse->x, prise_adverse->y, prise_adverse->newX, prise_adverse->newY, d2, couleur)){
	    			prise_adverse=prise_adverse->next;
	    		}
	    		else{
	    			return 0;
	    		}
	    	}
	    	return 1;
	}
   }
   
   //si il y a une prise avec la dame
   else if(ppr2!=NULL){
   	    if(prise_adverse->nbPrises != ppr2->nbPrises){
	  	return 0;
	    }
	    damier *d2 = damierFromDamier(d);
	    while(prise_adverse!=NULL){
	    	if(eatDame(prise_adverse->x, prise_adverse->y, prise_adverse->newX, prise_adverse->newY, d2, couleur)){
	    		prise_adverse=prise_adverse->next;
	    	}
	    	else{
	    		return 0;
	    	}
	    }
	    return 1;
  }
  return 0;
    	
}

// verifie le mouvement d'une piece ou d'une dame de l'adversaire, retourne 1 si valide, sinon 0
int verificationMoveAdverse(damier *d, char * coup, int couleur){
	if(strchr(coup, '-')==NULL){
    		return 0;
    	}
    	pprise ppr=maxPrise(d, couleur);
	pprise ppr2=maxPriseDame(d, couleur);
	if(ppr2!=NULL){
		if((ppr2->nbPrises <=0)||(ppr2->nbPrises >25)){
		    ppr2=NULL;
		}
	}
	if((ppr!=NULL)||(ppr2!=NULL)){
		return 0;
	}
	int * f = convertToMove(coup, d);
	if(checkLegalMove(f[0], f[1], f[2], f[3], d, couleur)){
		free(f);
		return 1;
	}
	else if(checkLegalMoveDame(f[0], f[1], f[2], f[3], d, couleur)){
		free(f);
		return 1;
	}
		
	free(f);
	return 0;
}

// effectue le coup adverse en verifiant si le coup est valide dans le damier local et retourne 1 si le coup est joué, sinon 0
int jouerCoupAdverse(damier *d, char * coup, int couleur){
	
	// on verifie si le mouvement de l'adversaire est valide et l'effectue
	if(strchr(coup, '-')!=NULL){
		if(verificationMoveAdverse(d, coup, couleur)){
			int * f = convertToMove(coup, d);
			if(move(f[0], f[1], f[2], f[3], d, couleur))
				printf("\n");
			else
				moveDame(f[0], f[1], f[2], f[3], d, couleur);
			d->coups++;
			return 1;
		}else{
			return 0;
		}
	/*}else if(strchr(coup, 'x')!=NULL){
		pprise p = convertToPrise(coup, d);
		if(d->plateau[p->x][p->y]->p->type){
			while(p!=NULL){
				eatDame(p->x, p->y, p->newX, p->newY, d, couleur);
				d->coups++;
				p=p->next;
			}
			return 1;
		}else{
			printf(" c'est incroyable \n");
			while(p!=NULL){
				eat(p->x, p->y, p->newX, p->newY, d, couleur);
				d->coups++;
				p=p->next;
			}
			return 1;
		}
	}*/

	// on verifie si la prise de l'adversaire est valide et l'effectue
	}else if(strchr(coup, 'x')!=NULL){
		if(verificationPriseAdverse(d, coup, couleur)){
			pprise p = convertToPrise(coup, d);
			if(d->plateau[p->x][p->y]->p->type){
				while(p!=NULL){
					eatDame(p->x, p->y, p->newX, p->newY, d, couleur);
					d->coups++;
					p=p->next;
				}
			}else{
				while(p!=NULL){
					eat(p->x, p->y, p->newX, p->newY, d, couleur);
					d->coups++;
					p=p->next;
				}
			}
			return 1;
			
		}else{
			return 0;
		}
    	}
	return 0;
}
