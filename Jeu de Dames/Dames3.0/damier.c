#include "damier.h"
#include <math.h>
#include <unistd.h> 

//initialise le damier de départ comme il se doit 
damier * damier_init(){
	damier * res = malloc(sizeof(damier));
	res->nbNoir=20;
	res->nbBlanc=20;
	res->coups=0;
	int f=1;
	int countN=1; // pour compter les cases noires et initaliser les index
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
		    if(f){
		    	res->plateau[i][j]=case_init_vide(f,-1);
		    }
		    else{
		  		res->plateau[i][j]=case_init_vide(f,countN);
		  		countN++;
		    }
		    f=!f;
		}
		f=!f;
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<10;j++){
			if(!(res->plateau[i][j]->fond)){
				res->plateau[i][j]->p=piece_init(0,0);
			}
		}
	}
	for(int i=6;i<10;i++){
		for(int j=0;j<10;j++){
			if(!(res->plateau[i][j]->fond)){
				res->plateau[i][j]->p=piece_init(0,1);
			}
		}
	}
	return res;
}

//crée un damier à partir d'un damier
damier * damierFromDamier(damier *d){
	damier * res = malloc(sizeof(damier));
	res->nbNoir=d->nbNoir;
	res->nbBlanc=d->nbBlanc;
	res->coups=d->coups;
	
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			if(d->plateau[i][j]->p!=NULL)
				res->plateau[i][j]=case_init(d->plateau[i][j]->p->type,d->plateau[i][j]->p->couleur,d->plateau[i][j]->fond,d->plateau[i][j]->index);
			else
				res->plateau[i][j]=case_init_vide(d->plateau[i][j]->fond,d->plateau[i][j]->index);
		}
	}
	return res;
}

// affiche le damier (cases blanches en 0, pions noirs en □ , pions blancs en ▪)
// Dames noires en N, Dames blanches en B
void affichage(damier * d){
	printf("-- ");
	for(int k=0; k<10;k++){
		printf("%d ",k);
	}
	printf("\n   ");
	
	for(int k=0; k<10;k++){
		printf("| ");
	}
	printf("\n");
	for(int i=0;i<10;i++){
		printf("%d--", i);
		for(int j=0;j<10;j++){
			if(d->plateau[i][j]->p==NULL){
				printf("0 ");
			}
			else if(!(d->plateau[i][j]->p->couleur)){
				if(d->plateau[i][j]->p->type)
					printf("N ");
				else 
					printf("□ ");
			}
			else{
				if(d->plateau[i][j]->p->type)
					printf("B ");
				else
					printf("▪ ");
			}
		}
		printf("\n");
	}
}

// affiche le damier (cases avec la notation de Manoury)
// Dames noires en N, Dames blanches en B
void affichage_numero_case(damier *d){
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			printf("%d ",d->plateau[i][j]->index);
		}
		printf("\n");
	}
}

// verifie si le mouvement est valide et renvoie 1 si mouvement valide, sinon 0
int checkLegalMove(int curX, int curY, int newX, int newY,damier * d, int couleur){
	// si l'une des cases source ou destination est hors du plateau 
	if((curX<0)||(curX>9)||(curY<0)||(curY>9)||(newX<0)||(newX>9)||(newY<0)||(newY>9)){
		printf("En dehors du damier\n");
		return 0;
	}
	
	// si il n'y a pas de piece a deplacer
	if(d->plateau[curX][curY]->p == NULL){
		return 0;
	}
	
	// si la pièce n'appartient pas au joueur
	if(couleur!=d->plateau[curX][curY]->p->couleur){
		return 0;
	}
	// si la case destination est déjà occupée
	if(d->plateau[newX][newY]->p!=NULL){
		return 0;
	}
	//mauvaise ligne 
	if(abs(newY - curY) != 1){
		return 0;
	}
	if(couleur){
	//mauvaise ligne
		if(curX - newX !=1){
			return 0;
		}
	}
	else{
		//mauvaise colonne
		if(newX - curX!=1){
			return 0;
		}
	}
		

	return 1;
}

//effectue le mouvement d'une piece, retourne 1 si le mouvement est effectué, sinon 0
int move(int curX, int curY, int newX, int newY,damier * d, int couleur){
	if (checkLegalMove(curX,curY,newX,newY,d,couleur)){
		modifier_piece(retirer_piece(d->plateau[curX][curY]),d->plateau[newX][newY]);
		d->coups++;
		if(couleur){
			if(newX==0){
				transformToDame(d->plateau[newX][newY]->p);
			}
		}
		else{
			if(newX==9){
				transformToDame(d->plateau[newX][newY]->p);
			}
		}
		return 1;
	}else{
		return 0;
	}
}

// verifie si la prise est valide et renvoie 1 si prise valide, sinon 0
int checkLegalEat(int curX, int curY, int newX, int newY, damier * d, int couleur){
	// si l'une des cases source ou destination est hors du plateau 
	if((curX<0)||(curX>9)||(curY<0)||(curY>9)||(newX<0)||(newX>9)||(newY<0)||(newY>9)){
		return 0;
	}
	
	// si il n'y a pas de piece a deplacer
	if(d->plateau[curX][curY]->p == NULL){
		return 0;
	}
	
	// si la pièce n'appartient pas au joueur
	if(couleur!=d->plateau[curX][curY]->p->couleur){
		return 0;
	}
	// si la case destination est déjà occupée
	if(d->plateau[newX][newY]->p!=NULL){
		return 0;
	}
	//si on essaie de se déplacer de 3 cases ou plus, ou si ce n'est pas en diagonale 
	if((abs(curX - newX) != 2) || (abs(curY - newY) != 2 )){
		return 0;
	}
	
	return 1;
}

// effectue la prise par une piece
int eat(int curX, int curY, int newX, int newY, damier * d, int couleur){
	if (checkLegalEat(curX,curY,newX,newY,d,couleur)){
		if(newY > curY && newX > curX){
			// si la piece que tu manges est la tienne ou si il n y a pas de piece
			if((d->plateau[curX + 1][curY + 1]->p == NULL) || (couleur == d->plateau[curX + 1][curY + 1]->p->couleur)){
				return 0;
			}
			retirer_piece(d->plateau[curX + 1][curY + 1]);
		}
		if((newY > curY) && (newX < curX)){
			// si la piece que tu manges est la tienne ou si il n y a pas de piece
			if((d->plateau[curX - 1][curY + 1]->p == NULL) || (couleur == d->plateau[curX - 1][curY + 1]->p->couleur)){
				return 0;
			}
			retirer_piece(d->plateau[curX - 1][curY + 1]);
		}
		if((newY < curY) && (newX > curX)){
			// si la piece que tu manges est la tienne ou si il n y a pas de piece
			if((d->plateau[curX + 1][curY - 1]->p == NULL) || (couleur == d->plateau[curX + 1][curY - 1]->p->couleur)){
				return 0;
			}
			retirer_piece(d->plateau[curX + 1][curY - 1]);
		}
		if((newY < curY) && (newX < curX)){
			// si la piece que tu manges est la tienne ou si il n y a pas de piece
			if((d->plateau[curX - 1][curY - 1]->p == NULL) || (couleur == d->plateau[curX - 1][curY - 1]->p->couleur)){
				return 0;
			}
			retirer_piece(d->plateau[curX - 1][curY - 1]);
		}
		
		modifier_piece(retirer_piece(d->plateau[curX][curY]), d->plateau[newX][newY]);
		d->coups++;
		
		if(couleur){
			d->nbNoir--;
			if(newX==0){
				transformToDame(d->plateau[newX][newY]->p);
			}
		}
		else{
			d->nbBlanc--;
			if(newX==9){
				transformToDame(d->plateau[newX][newY]->p);
			}
		}
		return 1;
	}
	else{
		return 0;
	}
	
}

// transforme une piece en dame
void transformToDame(piece * p){
	p->type = 1;
}

// verifie si le mouvement d'une dame est valide
int checkLegalMoveDame(int curX, int curY, int newX, int newY, damier *d , int couleur){
	// si l'une des cases source ou destination est hors du plateau 
	if((curX<0)||(curX>9)||(curY<0)||(curY>9)||(newX<0)||(newX>9)||(newY<0)||(newY>9)){
		return 0;
	}
	// si la pièce sur la case est NULL 
	if(d->plateau[curX][curY]->p==NULL){
		return 0;
	}
	// si la pièce sur la case n'appartient pas au joueur
	if(d->plateau[curX][curY]->p->couleur != couleur){
		return 0;
	}
	// si la pièce sur la case n'est pas une dame 
	if(!(d->plateau[curX][curY]->p->type)){
		return 0;
	}
	// si le coup n'est pas en diagonale 
	if(abs(curX - newX) != abs(curY - newY)){
		return 0;
	}
	// si la case destination est déjà occupée
	if(d->plateau[newX][newY]->p!=NULL){
		return 0;
	}
	
	int i = curX;
	int j = curY;
	// on parcourt toutes le cases de la case courante à la case destination
	while( (i != newX) || (j!=newY)){
		if(newX > i)
			i++;
		else 
			i--;
		if(newY > j)
			j++;
		else
			j--;
		//si l'une des cases n'est pas vide 
		if(d->plateau[i][j]->p!=NULL){
			return 0;
		}
	}
	return 1;

}

// effectue le mouvement d'une dame
int moveDame(int curX, int curY, int newX, int newY, damier *d, int couleur){
	if(checkLegalMoveDame(curX, curY, newX, newY, d, couleur)){
		modifier_piece(retirer_piece(d->plateau[curX][curY]),d->plateau[newX][newY]);
		d->coups++;
		return 1;
	}
	return 0;
}

// verifie si une dame peut effectuer une prise
int checkEatDame(int curX, int curY, int newX, int newY, damier *d, int couleur){
	// si l'une des cases source ou destination est hors du plateau 
	if((curX<0)||(curX>9)||(curY<0)||(curY>9)||(newX<0)||(newX>9)||(newY<0)||(newY>9)){
		return 0;
	}
	//si les coordonnées de départ sont les mêmes que celles d'arrivé 
	if((curX==newX)&&(curY==newY)){
		return 0;
	}
	// si la pièce sur la case est NULL 
	if(d->plateau[curX][curY]->p==NULL){
		return 0;
	}
	// si la pièce sur la case n'appartient pas au joueur
	if(d->plateau[curX][curY]->p->couleur != couleur){
		return 0;
	}
	// si la pièce sur la case n'est pas une dame 
	if(!(d->plateau[curX][curY]->p->type)){
		return 0;
	}
	// si le coup n'est pas en diagonale 
	if(abs(curX - newX) != abs(curY - newY)){
		return 0;
	}
	// si la case destination est déjà occupée
	if(d->plateau[newX][newY]->p!=NULL){
		return 0;
	}
	int i = curX;
	int j = curY;
	int conteur = 0;
	// on parcourt toutes le cases de la case courante à la case destination
	while( (i != newX) || (j!=newY)){
		if(newX > i)
			i++;
		else 
			i--;
		if(newY > j)
			j++;
		else
			j--;
		//si l'une des cases n'est pas vide 
		if(d->plateau[i][j]->p!=NULL){
			if(d->plateau[i][j]->p->couleur==couleur){
				return 0;
			}else{
				conteur++;
			}
		}
	}
	return (conteur == 1);
	
}

// effectue la prise par une dame
int eatDame(int curX, int curY, int newX, int newY, damier *d , int couleur){
	if(checkEatDame(curX,curY, newX, newY, d, couleur)){
		int i = curX;
		int j = curY;
		// on parcourt toutes le cases de la case courante à la case destination
		while((i != newX) || (j!=newY)){
			if(newX > i)
				i++;
			else 
				i--;
			if(newY > j)
				j++;
			else
				j--;
			//si l'une des cases n'est pas vide 
			if(d->plateau[i][j]->p!=NULL){
				retirer_piece(d->plateau[i][j]);
			}
		}
		modifier_piece(retirer_piece(d->plateau[curX][curY]),d->plateau[newX][newY]);
		d->coups++;
		return 1;
	}
	return 0;
}

// renvoie le maximum d'un tableau (utilisé dans maxPriseCase)	
int max(int * tab, int n){
	int m = tab[0];
	for(int i=1;i<n;i++){
		if(tab[i]>m)
			m=tab[i];
	}
	return m;
}

// met dans une struct pprise (liste chainée) les meilleures prises pouvant etre effectué dans le damier par tous les pions
pprise maxPriseCase(damier *d, int x, int y,int couleur){
	damier * da=damierFromDamier(d);
	pprise res;
	/*out of range*/
	if((x<0)||(x>9)||(y<0)||(y>9)){
		printf("out of range\n");
		return NULL;
	}
	pprise * cur =malloc(sizeof(pprise)*4);
	int * tab=malloc(sizeof(int)*4);
	for(int i=0; i<4;i++){
		cur[i]=malloc(sizeof(struct prises));
	}
	if(eat(x,y,x-2,y-2,da,couleur)){
		cur[0]->x=x;
		cur[0]->y=y;
		cur[0]->newX=x-2;
		cur[0]->newY=y-2;
		cur[0]->next=maxPriseCase(da,x-2,y-2,couleur);
		if(cur[0]->next!=NULL){
			cur[0]->nbPrises=1+cur[0]->next->nbPrises;
		}
		else{ 
			cur[0]->nbPrises=1;
		}
		tab[0]=cur[0]->nbPrises;
	}
	else{
		tab[0]=0;
		free(cur[0]);
		cur[0]=NULL;
	}
	if(eat(x,y,x-2,y+2,da,couleur)){
		cur[1]->x=x;
		cur[1]->y=y;
		cur[1]->newX=x-2;
		cur[1]->newY=y+2;
		cur[1]->next=maxPriseCase(da,x-2,y+2,couleur);
		if(cur[1]->next!=NULL){
			cur[1]->nbPrises=1+cur[1]->next->nbPrises;
		}
		else{ 
			cur[1]->nbPrises=1;
		}
		tab[1]=cur[1]->nbPrises;
	}else{
		tab[1]=0;
		free(cur[1]);
		cur[1]=NULL;
	}
	if(eat(x,y,x+2,y+2,da,couleur)){
		cur[2]->x=x;
		cur[2]->y=y;
		cur[2]->newX=x+2;
		cur[2]->newY=y+2;
		cur[2]->next=maxPriseCase(da,x+2,y+2,couleur);
		if(cur[2]->next!=NULL){
			cur[2]->nbPrises=1+cur[2]->next->nbPrises;
		}
		else{ 
			cur[2]->nbPrises=1;
		}
		tab[2]=cur[2]->nbPrises;
		
	}
	else{
		tab[2]=0;
		free(cur[2]);
		cur[2]=NULL;
	}
	if(eat(x,y,x+2,y-2,da,couleur)){
		cur[3]->x=x;
		cur[3]->y=y;
		cur[3]->newX=x+2;
		cur[3]->newY=y-2;
		cur[3]->next=maxPriseCase(da,x+2,y-2,couleur);
		if(cur[3]->next!=NULL){
			cur[3]->nbPrises=1+cur[3]->next->nbPrises;
		}
		else{ 
			cur[3]->nbPrises=1;
		}
		tab[3]=cur[3]->nbPrises;
	}
	else{
		tab[3]=0;
		free(cur[3]);
		cur[3]=NULL;
	}
	if((cur[0]==NULL)&&(cur[1]==NULL)&&(cur[2]==NULL)&&(cur[3]==NULL)){
		return NULL;
	}
	int maxP=max(tab,4);
	if(cur[0]!=NULL){
		if(cur[0]->nbPrises==maxP){
			res=cur[0];
		}
	}
	if(cur[1]!=NULL){
		if(cur[1]->nbPrises==maxP){
			res=cur[1];
		}
	}
	if(cur[2]!=NULL){
		if(cur[2]->nbPrises==maxP){
			res=cur[2];
		}
	}
	if(cur[3]!=NULL){
		if(cur[3]->nbPrises==maxP){
			res=cur[3];
		}
	}
	return res;
}

// met dans une struct pprise (liste chainée) les meilleures prises pouvant etre effectué dans le damier par toutes les dames 
pprise maxPriseCaseDame(damier *d, int x, int y, int couleur){
	damier * da=damierFromDamier(d);
	pprise res;
	/*out of range*/
	if((x<0)||(x>9)||(y<0)||(y>9)){
		printf("out of range\n");
		return NULL;
	}
	pprise * cur = malloc(sizeof(pprise)*50);
	int * tab=malloc(sizeof(int)*50);
	for(int i=0; i<50;i++){
		cur[i]=malloc(sizeof(struct prises));
	}
	int count=0;
	
	// verifie a chaques cases le nombre de prises possible d'une dame
	for(int k=0;k<10;k++){
		for(int l=0;l<10;l++){
			if(da->plateau[k][l]->fond==0){
				if(eatDame(x,y,k,l,da,couleur)){
					cur[count]->x=x;
					cur[count]->y=y;
					cur[count]->newX=k;
					cur[count]->newY=l;
					cur[count]->next=maxPriseCaseDame(da,k,l,couleur);
					if(cur[count]->next!=NULL){
						cur[count]->nbPrises=1+cur[count]->next->nbPrises;
					}
					else{ 
						cur[count]->nbPrises=1;
					}
					tab[count]=cur[count]->nbPrises;
				}
				else{
					tab[count]=0;
					free(cur[count]);
					cur[count]=NULL;
				}
				count++;
			}
		}
	}
	int i=0;
	
	while(i<50){
		if(cur[i]==NULL){
			i++;
		}
		else{
			break;
		}
		if(i==49){
			free(cur);
			return NULL;
		}
	}
	int maxP = max(tab,50);
	if((maxP<=0)||(maxP>25)){
		return NULL;
	}
	for(i=0;i<50;i++){
		if(cur[i]!=NULL){
			if(cur[i]->nbPrises==maxP){
				res=cur[i];
			}
		}
	}
	return res;
}


// retourne la meilleure prise possible d'une piece						
pprise maxPrise(damier* d, int couleur){
	pprise * tout;
	int * tab;
	int taille;
	
	if(couleur){
		tout=malloc(sizeof(pprise)*(d->nbBlanc));
		for(int k=0; k<d->nbBlanc;k++){
			tout[k]=malloc(sizeof(struct prises));
		}
		tab=malloc(sizeof(int)*(d->nbBlanc));
		taille=d->nbBlanc;
	}
	else{
		tout=malloc(sizeof(pprise)*(d->nbNoir));
		for(int k=0; k<d->nbNoir;k++){
			tout[k]=malloc(sizeof(struct prises));
		}
		tab=malloc(sizeof(int)*(d->nbNoir));
		taille=d->nbNoir;
	}
	
	int cur=0;
	int l=0;
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			if(d->plateau[i][j]->p!=NULL){
				if(d->plateau[i][j]->p->couleur==couleur){
					tout[cur]=maxPriseCase(d,i,j,couleur);
					if(tout[cur]==NULL){
						tab[l]=0;
					}
					else{
						tab[l]=tout[cur]->nbPrises;
						
					}
					l++;
					cur++;
				}
			}
		}
	}
	int mp=max(tab,taille);
	for(int i=0;i<taille;i++){
		if(tout[i]!=NULL){
			if(tout[i]->nbPrises==mp){
				return tout[i];
			}
		}
	}
	
	return NULL;
}

// retourne la meilleure prise possible d'une dame	
pprise maxPriseDame(damier *d, int couleur){
	pprise * tout;
	int * tab;
	int taille;
	if(couleur){
		tout=malloc(sizeof(pprise)*(d->nbBlanc));
		for(int k=0; k<d->nbBlanc;k++){
			tout[k]=malloc(sizeof(struct prises));
		}
		tab=malloc(sizeof(int)*(d->nbBlanc));
		taille=d->nbBlanc;
	}
	else{
		tout=malloc(sizeof(pprise)*(d->nbNoir));
		for(int k=0; k<d->nbNoir;k++){
			tout[k]=malloc(sizeof(struct prises));
		}
		tab=malloc(sizeof(int)*(d->nbNoir));
		taille=d->nbNoir;
	}
	int cur=0;
	int l=0;
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			if(d->plateau[i][j]->p!=NULL){		
				if((d->plateau[i][j]->p->couleur==couleur)&&(d->plateau[i][j]->p->type)){				
					tout[cur]=maxPriseCaseDame(d,i,j,couleur);
					if(tout[cur]==NULL){
						tab[l]=0;
					}
					else{
						tab[l]=tout[cur]->nbPrises;	
					}
					l++;
					cur++;
				}
			}
		}
	}
	int mp=max(tab,taille);
	for(int i=0;i<taille;i++){
		if(tout[i]!=NULL){
			if(tout[i]->nbPrises==mp){
				return tout[i];
			}
		}
	}
	
	return NULL;
}
void prise_destroy(pprise p){
	
}
void damier_destroy(damier *d){
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			case_destroy(d->plateau[i][j]);
		}
	}
	free(d);
	d=NULL;
}
