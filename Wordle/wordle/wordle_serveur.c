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
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <signal.h>
#include "exact_rw.h"
#include "mots_5_lettres.h"

#define PORT_WRDLP 4242
typedef struct{
	int sock;
	char *prop;
	pthread_mutex_t *majlog_clt;
	char reponse[6];
	int log;
}wrdlpClt;

pthread_mutex_t majlog;

/* Met toutes les lettres minuscules présentes dans ch en majuscule (les autres
 * caractères sont inchangés).
 * Précondition : ch contient un caractère nul qui sert de délimiteur de fin */
void chaine_toupper(char *ch);

/* Vider le tampon de l'entrée standard */
void vider_tampon();

/* Met dans la chaine prop (tableau d'au moins 6 char) un mot de 5 lettres saisi
 * par l'utilisateur, si besoin mis en majuscule, et terminé par un '\0'.
 * Redemande la saisie tant que
 * - le mot de l'utilisateur a moins de 5 lettres ou
 * - n'est pas dans la liste de mots.
 * Si l'utilisateur saisit un mot de plus de 5 lettres, seules les 5 premières
 * sont prises en compte.
 */
void *saisir_prop(void *prop);

/* Les 5 premières lettre de prop_joueur sont stockées dans un tableau checkWord :
 * * cette lettre en majuscule si elle figure à la même position dans a_deviner
 * * cette lettre en minuscule si elle fait partie de a_deviner mais à une autre
 *   position
 * * le caractère _ sinon
 * L'affichage se termine par un saut de ligne.
 * Retourne le tableau checkWord
 * Préconditions : prop_joueur et a_deviner contiennent au moins 5 caractères
 *                 qui sont tous des lettres majuscules */
char *traiter_prop(const char *prop_joueur, const char *a_deviner, int *nb_lettres_trouvees);

int main(){
	pthread_mutex_init(&majlog, NULL);
	pthread_t th;
	/*Pour éviter la casse du serveur... */
	signal(SIGPIPE, SIG_IGN);
	/* Création d'un journal de bord */
	int log = open("wrdlp.log",O_WRONLY |O_TRUNC | O_CREAT, 0644);
	if(log < 0) {
		perror("wordle.log open\n");
		exit(2);
	}
	/* Création d'une socket ipv4 */
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("socket");
		exit(2);
	}
	/* Création de la sockaddr locale */
	struct sockaddr_in sa = { .sin_family = AF_INET,
				  .sin_port = htons(PORT_WRDLP),
				  .sin_addr.s_addr = htonl(INADDR_ANY) };
	/* Attacher la socket à l'adresse locale */
	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
	if(bind(sock, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		perror("bind");
		exit(3);
	}
	
	for(;;){
		if(listen(sock, 16) < 0) {
			perror("listen");
			exit(2);
		}
		struct sockaddr_in addr_clt;
		char mot[6];
		srand(time(NULL));
		mot_alea5(mot);
		socklen_t taille_addr = sizeof(struct sockaddr_in);
		int sock_e = accept(sock,(struct sockaddr *)&addr_clt, &taille_addr);
		if(sock_e<0){
			perror("accept : ");
			return 1;
		}
		char addr_char[INET_ADDRSTRLEN];
		if(inet_ntop(AF_INET, &(addr_clt.sin_addr), addr_char, INET_ADDRSTRLEN) == NULL){
			perror("inet_ntop");
		}
		else{
			char date[32];
			time_t start = time(NULL);
			strftime(date, 32, "%F à %T", localtime(&start));
			char logmsg[256];
			sprintf(logmsg, "%s\t-- Connexion depuis : %s\n", date, addr_char);
			pthread_mutex_lock(&majlog);
			exact_write(log, logmsg, strlen(logmsg));
			pthread_mutex_unlock(&majlog);
		}
		wrdlpClt *propClt = malloc(sizeof(wrdlpClt));
		propClt->sock = sock_e;
		strcpy(propClt->reponse, mot);
		propClt->majlog_clt = &majlog;
		propClt->log = log;
		
		if(pthread_create(&th, NULL, saisir_prop, propClt) < 0){
			perror("pthread create");
			return 1;
		}
		pthread_detach(th);
	}
	pthread_mutex_destroy(&majlog);
	close(sock);
	return 0;
}

void chaine_toupper(char *ch){
	int i;
	for (i = 0; ch[i] != '\0'; i = i + 1)
		ch[i] = toupper(ch[i]);
}

char *traiter_prop(const char *prop_joueur, const char *a_deviner, int *nb_lettres_trouvees){
	char *checkWord = malloc(6*sizeof(char));
	int i;
	*nb_lettres_trouvees = 0;
	for (i = 0; i < 5; i = i + 1) {
		if (prop_joueur[i] == a_deviner[i]) {
			checkWord[i] = prop_joueur[i];
			*nb_lettres_trouvees = *nb_lettres_trouvees + 1;
		} else if (strchr(a_deviner, prop_joueur[i])){
			checkWord[i] = tolower(prop_joueur[i]);
		} else {
			checkWord[i] = '_';
		}
	}
	checkWord[5] = '\0';
	return checkWord;
}

void vider_tampon(){
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

void *saisir_prop(void *prop){
	wrdlpClt *propClt = (wrdlpClt *)prop;
	propClt->prop = malloc(6*sizeof(char));
	int correct = 0;
	int nb_lettres_trouvees;
	int nb_tentatives = 0;
	char *check;
	while(1){
		/* Affichage du mot à deviner côté serveur seulement 
		(sinon trop facile pour le joueur) */
		printf("Mot à deviné : %s\n", propClt->reponse); 
		if(exact_read(propClt->sock, propClt->prop, 5*sizeof(char)) < 0){
			perror("exact_read");
			exit(2);
		}
		time_t start = time(NULL);
		char date[32];
		strftime(date, 32, "%F à %T", localtime(&start));
		char logmsg[256];
		sprintf(logmsg, "%s:\tProposition : %s\tMot à deviner : %s\n", date, propClt->prop, propClt->reponse);
		nb_tentatives++;
		/* Bloquage de log */
		pthread_mutex_lock(&majlog); 
		/* Écriture dans le log */
		exact_write(propClt->log, logmsg, strlen(logmsg)); 
		/* Débloquage de log */
		pthread_mutex_unlock(&majlog); 
		
		/* Vérification de la proposition du joueur */
		if(strlen(propClt->prop) < 5)
			check = "Le mot à deviner est à 5 lettres!\n";
		else{
			chaine_toupper(propClt->prop);
			if(!est_dans_liste_mots(propClt->prop))
				check = "Ce mot n'est pas dans la liste des mots!\n";
			else{
				/* Vérification du nombre de lettres correctes */
				check = traiter_prop(propClt->prop, propClt->reponse, &nb_lettres_trouvees);
				if(nb_lettres_trouvees == 5){
					/* Si toutes les lettres sont correctes */
					check = "Bravo, tu as trouvé le mot!\n";
					correct = 1;
					/* Écriture avec mutex dans le log du nombre de tentatives */
					sprintf(logmsg, "\tNombre de tentatives : %d\n", nb_tentatives);
					pthread_mutex_lock(&majlog);
					exact_write(propClt->log, logmsg, strlen(logmsg));
					pthread_mutex_unlock(&majlog);
				}
			}
		}
	/* Sans la vérification fait une boucle infinie chez le serveur.
	   Sans return NULL nous donne Erreur "Broken pipe" et arrête le serveur.
	   Et ce quand un client fait Ctrl+C */
		if(exact_write(propClt->sock, check, 256*sizeof(char)) < 0)
			return NULL; 
		if(correct){
			exact_read(propClt->sock, propClt->prop, 5*sizeof(char));
			/* Vérification que le joueur veut rejouer */
			if(!strcmp(propClt->prop, "nwrun")){
				/* Remise à 0 des variables */
				correct = 0;
				nb_lettres_trouvees = 0;
				nb_tentatives = 0;
				/* Repioche un mot aléatoirement */
				srand(time(NULL));
				mot_alea5(propClt->reponse);
			}
			/* S'il ne veut pas rejouer alors on sort de while */
			else 
				break;
		}
		
	}
	/* Ferme la socket du client */
	close(propClt->sock); 
	return NULL;
}
