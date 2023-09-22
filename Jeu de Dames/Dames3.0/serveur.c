#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/stat.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "damier.h"
#include "randomPlayer.h"
#include "exact_rw.h"
#include "adverse.h" 
#include "envoi.h" 
#include <signal.h> 
#include "enregistrement.h"
#define PORT_INCP 7777
#define PORT_RAPPORT 5777
#define MAX 256

typedef struct{
    damier *d;       // pour stocker le damier
    randomPlayer *player; // pour y stocker le joueur et sa couleur
    int sock;   // pour la socket d'échange 
    int logFile;          // pour y stocker le descripteur de fichier log
}Client;

//transforme une chaîne de aractère hexadécimale en un entier sur 8 bit  
uint8_t hex2int(char *hex) {
    uint8_t val = 0;
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++; 
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}
int rapportPartie(char * arg){
	int logFile;
	char str[50] = "2001:910:1410:523:0:fada:80af:2bc2\0";
	if((logFile=open(arg, O_RDONLY, 0644))<0){
		perror("open : ");
		return 1;
	}
	uint8_t tab[256] = {0};
	char * buff = malloc(sizeof(char)*2);
	int i=0;
	while(read(logFile, buff, sizeof(char)*2)!=0){
		tab[i]=hex2int(buff);
		i++;
	}
	int sock;
	if((sock=socket(AF_INET6, SOCK_STREAM, 0))<0){
		perror("socket");
		return 1;
	}
	/* Création de la sockaddr */
	struct sockaddr_in6 sa = { .sin6_family = AF_INET6, .sin6_port = htons(PORT_RAPPORT) };
	if (inet_pton(AF_INET6, str, &sa.sin6_addr) != 1) {
		fprintf(stderr, "adresse ipv6 non valable\n");
		exit(1);
	}
	if (connect(sock, (struct sockaddr *) &sa, sizeof(struct sockaddr_in6)) < 0) {
		perror("connect");
		exit(3);
	}
	exact_write(sock,tab, sizeof(uint8_t)*256);
	char str2[256];
	for(int i=0; i<256;i++){
		printf("%d ", tab[i]);
	}
	printf("\n");
	read(sock, str2, sizeof(char)*256);
	printf(" %s \n", str2);
	close(sock);
	free(buff);
	close(logFile);
	
	return 0;
}
// ecrit dans le logFile l'adresse IPv6 voulue (avec les 0 si besoin)
void ipv6_expander(const struct in6_addr * addr, int logFile)
{
                char str[33];
                sprintf(str,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                (int)addr->s6_addr[0], (int)addr->s6_addr[1],
                (int)addr->s6_addr[2], (int)addr->s6_addr[3],
                (int)addr->s6_addr[4], (int)addr->s6_addr[5],
                (int)addr->s6_addr[6], (int)addr->s6_addr[7],
                (int)addr->s6_addr[8], (int)addr->s6_addr[9],
                (int)addr->s6_addr[10], (int)addr->s6_addr[11],
                (int)addr->s6_addr[12], (int)addr->s6_addr[13],
                (int)addr->s6_addr[14], (int)addr->s6_addr[15]);
                 write(logFile, str, sizeof(char)*32);
}
void * jouer(void * arg);
int main()
{
	/* Création d'une socket tcp ipv6 */
	int sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(2);
	}
	int logFile;
	if((logFile = open("serveur.log", O_CREAT | O_TRUNC |O_WRONLY, S_IREAD | S_IWRITE))<0){
		perror("open :");
		return 1;
	}
	int sock_echange;
	/* Création de la sockaddr */
	struct sockaddr_in6 sa; 
	sa.sin6_family = AF_INET6; 
	sa.sin6_port = htons(PORT_INCP); 
	sa.sin6_addr = in6addr_any;
	/* Réutiliser l'adresse sans attendre après la fin du serveur. */
	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
  	signal(SIGPIPE, SIG_IGN);
  	if (bind(sock, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		perror("bind");
		exit(3);
	}

	/* 4. Écouter sur la socket d'écoute */
	if (listen(sock, 10) < 0) {
		perror("listen");
		exit(2);
	}
  	char str[2]="ff";
  	write(logFile, str, sizeof(char)*2);
   	while(1){
   		pthread_t t;
	    	damier * d = damier_init();
		randomPlayer * p2 = randomPlayer_init(1);
		struct sockaddr_in6 cli;
		socklen_t len = sizeof(cli);
		
		/* 5. Attente passive d'une connection. */
		sock_echange = accept(sock, (struct sockaddr *)&cli, &len);
		ipv6_expander(&cli.sin6_addr, logFile);
		if(sock_echange < 0){
		    printf("accept fail ....\n");
		    exit(0);
		}
		Client c = {.d=d, .player=p2, .sock=sock_echange, .logFile=logFile};
		pthread_create(&t, NULL, jouer, &c);
		pthread_detach(t);
	}
		
	close(sock);
	return 0;	
}

// fonction jouer pour le reseau 
void * jouer(void * arg)
{
    Client c = *((Client *) arg);
    damier *d = c.d;
    randomPlayer * p1 = c.player;
    int sock = c.sock;
    int logFile= c.logFile;
    int compteur;
    char * message= malloc(sizeof(char)*MAX);
    while((d->coups <= 100)&&(d->nbBlanc!=0)&&(d->nbNoir!=0))
    {
    	char *coupAdverse=malloc(sizeof(char)*MAX);
    	compteur=0;
	system("clear");
	affichage(d);
        play(p1, d, sock, logFile);
        read(sock, coupAdverse, sizeof(char)*MAX);
       	while(strcmp(coupAdverse, "COUP INVALIDE")==0){
       		printf("coupAdverse : %s \n", coupAdverse);
       		sleep(1);
       		play(p1, d, sock, logFile);
       		free(coupAdverse);
       		char *coupAdverse=malloc(sizeof(char)*MAX);
       		read(sock, coupAdverse, sizeof(char)*MAX);
       	}
       	printf("coupAdverse : %s \n", coupAdverse);
        sleep(1);
	while(jouerCoupAdverse(d,coupAdverse,0)==0){
		compteur++;
		if(compteur>=3){
			message="INTERUPTION";
		}else{
			message="COUP INVALIDE";
		}
		write(sock, message, sizeof(char)*strlen(message));
		free(coupAdverse);
       		char *coupAdverse=malloc(sizeof(char)*MAX);
		read(sock, coupAdverse, sizeof(char)*MAX);
	}
	if(strchr(coupAdverse, 'x')!=NULL){
		ecrirePrise(convertToPrise(coupAdverse, d), d, logFile);
	}
	else{
		int * f =convertToMove(coupAdverse, d);
		ecrireMove(f[0], f[1], f[2], f[3],d, logFile);
	}
	if(d->nbBlanc==0){
		message="PERDU";
		write(sock, message, sizeof(char)*strlen(message));
	}
	if(d->coups==100){
		message="EGALITE";
		write(sock, message, sizeof(char)*strlen(message));
	}
	free(coupAdverse);
    }
    randomPlayerDestroy(p1);
    damier_destroy(d);
    close(logFile);
    rapportPartie("serveur.log");
    return NULL;
}
