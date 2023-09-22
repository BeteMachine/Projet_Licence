/* Kévin CHEN 11807217
   Alexandre MARICIC 11803731
   Nous déclarons qu'il s'agit de notre propre travail. */
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "exact_rw.h"
#define PORT_WRDLP 4242

void usage(char *nom_prog)
{
	fprintf(stderr, "Usage: %s addr_ipv4\n"
			"client pour Wordle (Worlde Protocol)\n"
			"Exemple: %s 142.250.178.142\n", nom_prog, nom_prog);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}
	/* Création d'une socket ipv4 */
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(2);
	}
	/* Création de la sockaddr distante */
	struct sockaddr_in sa = { .sin_family = AF_INET, .sin_port = htons(PORT_WRDLP) };
	if (inet_pton(AF_INET, argv[1], &sa.sin_addr) != 1) {
		fprintf(stderr, "adresse ipv4 non valable\n");
		exit(1);
	}
	if (connect(sock, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) < 0) {
		perror("connect");
		exit(3);
	}
	char prop[6], retour[256];
	/* Échange avec le serveur */
	for(;;){
		/* Si le mot n'a pas encore été trouvé */
		if(strcmp(retour, "Bravo, tu as trouvé le mot!\n") != 0)
			printf("Votre proposition :");
		else{
			/* Si il a été trouvé */
			printf("On s'en refait une ? (o/n)\n");
			scanf("%s", prop);
			/* Si on ecrit autre chose que "o" ou "oui" */
			if((strcasecmp(prop, "O")^strcasecmp(prop, "OUI")) == 0) 
				break;
			else{ 
				/* Sinon */
				exact_write(sock, "nwrun", 5*sizeof(char));
				printf("Nouvelle partie !\nVotre proposition :");
			}
		}
		/* Construction de la requête Wordle */
		scanf("%s", prop);
		exact_write(sock, prop, 5*sizeof(char)); /* Envoie de la proposition */
		printf("%s\n",prop);
		/* Réponse du serveur */
		exact_read(sock, retour, 256*sizeof(char)); /* Retour du serveur */
		printf("%s\n", retour);
	}
	close(sock);
	return 0;
}
