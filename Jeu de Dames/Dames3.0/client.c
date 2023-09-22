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
#include "exact_rw.h"
#include "adverse.h"
#include "envoi.h"
#include "randomPlayer.h"
#include "enregistrement.h"
#define SERVER_PORT 7777
#define PORT_RAPPORT 5777
#define MAX 256
void jouer(int sock, damier *d, int logFile );
void usage(char *nom_prog)
{
	fprintf(stderr, "Usage: %s addr_ipv6\n"
			"client pour Jeu de Dame\n"
			"Exemple: %s 2001:910:1410:523:0:fada:80af:2bc2\n", nom_prog, nom_prog);
}
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

// pour rapporter la partie enregistré dans le fichier est arg
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

int main(int argc, char *argv[])
{
	if (argc != 2){
		usage(argv[0]);
		return 1;
	}
	int logFile;
	if((logFile = open("client.log", O_CREAT | O_TRUNC |O_WRONLY, S_IREAD | S_IWRITE))<0){
		perror("open :");
		return 1;
	}
	/* Création d'une socket tcp ipv6 */
	int sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(2);
	}
	/* Création de la sockaddr */
	struct sockaddr_in6 sa = { .sin6_family = AF_INET6, .sin6_port = htons(SERVER_PORT) };
	if (inet_pton(AF_INET6, argv[1], &sa.sin6_addr) != 1) {
		fprintf(stderr, "adresse ipv6 non valable\n");
		exit(1);
	}
	char str[2]="01";
	write(logFile, str, sizeof(char)*2);
	ipv6_expander(&sa.sin6_addr, logFile);
	/* Pour utiliser read et write */
	if (connect(sock, (struct sockaddr *) &sa, sizeof(struct sockaddr_in6)) < 0) {
		perror("connect");
		exit(3);
	}
	damier *d = damier_init();
	jouer(sock, d, logFile);
	close(sock);
	return 0;
}

// fonction jouer pour le reseau 
void jouer(int sock, damier *d, int logFile)
{
	randomPlayer * p1=randomPlayer_init(0);
	char * message = malloc(sizeof(char)*MAX);
	int compteur;
	while((d->coups<=100)&&(d->nbBlanc!=0)&&(d->nbNoir!=0)){
		char * coupAdverse= malloc(sizeof(char)*MAX);
		system("clear");
		affichage(d);
		compteur=0;
		read(sock, coupAdverse, sizeof(char)*MAX);
		while(strcmp(coupAdverse, "COUP INVALIDE")==0){
	       		printf("coupAdverse : %s \n", coupAdverse);
	       		sleep(1);
	       		play(p1, d, sock, logFile);
	       		free(coupAdverse);
	       		char * coupAdverse= malloc(sizeof(char)*MAX);
	       		read(sock, coupAdverse, sizeof(char)*MAX);
	       	}
		printf("coup Adverse : %s \n", coupAdverse);
		while(jouerCoupAdverse(d,coupAdverse,1)==0){
			compteur++;
			if(compteur>=3){
				message="INTERUPTION";
			}else{
				message="COUP INVALIDE";
			}
			write(sock, message, sizeof(char)*strlen(message));
			free(coupAdverse);
			char * coupAdverse= malloc(sizeof(char)*MAX);
			read(sock, coupAdverse, sizeof(char)*MAX);
		}
		if(strchr(coupAdverse, 'x')!=NULL){
			ecrirePrise(convertToPrise(coupAdverse, d), d, logFile);
		}
		else{
			int * f =convertToMove(coupAdverse, d);
			ecrireMove(f[0], f[1], f[2], f[3], d, logFile);
			free(f);
		}
		play(p1,d,sock, logFile);
		if(d->nbNoir==0){
			message="PERDU";
			write(sock, message, sizeof(char)*strlen(message));
		}
		if(d->coups==100){
			message="EGALITE";
			write(sock, message, sizeof(char)*strlen(message));
		}
		free(coupAdverse);
	}
	close(logFile);
	rapportPartie("client.log");
}
