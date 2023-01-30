#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include <pthread.h>

#define MAX_DIFF 10

typedef struct arg_struct{
	int *arg1;
	char **arg2;
	int *arg3;
}arg_struct;

/*réflechir au moyen de stocker les données dans les messages reçus*/

void *communication(void *arguments) {
    arg_struct args = *(struct arg_struct *) arguments;
    int so = *((int *) args.arg1);
    char **annuaire = (char **) args.arg2;
    int taille = *(int *) args.arg3;
    char buff[500]; /*METTRE LA BONNE TAILLE DE MESSAGES*/
    int recu = recv(so, buff, 499 * sizeof(char), 0);
    buff[recu] = '\0';
    while (1) {
        if (strcmp(strtok(buff, " "), "REGI") == 0) {
            /*requete d'un diffuseur pour s'enregistrer*/
            if (taille == MAX_DIFF) {
                send(so, "RENO", 4, 0);
                break;
            }
        }else{

        }
    }
    free(((struct arg_struct *)arguments)->arg1);
    close(so);
    return NULL;
}

int main(int argc, char** argv){
	if (argc != 2){
    	printf("Il faut fournir un numéro de port\n");
    	return 0;
  	}

 	unsigned int port_com = strtol(argv[1], NULL, 10);
 	char** annuaire= malloc(MAX_DIFF * sizeof(char*));
 	int taille= 0;
 	int sock= socket(PF_INET, SOCK_STREAM, 0);
 	struct sockaddr_in adress_sock; 
 	adress_sock.sin_family= AF_INET;
 	adress_sock.sin_port = htons(port_com);
 	adress_sock.sin_addr.s_addr= htonl(INADDR_ANY);
 	int r= bind(sock, (struct sockaddr*) &adress_sock, sizeof(struct sockaddr_in));
 	if(r == 0){
 		r= listen(sock, 0);
 		while(1){
 			struct sockaddr_in caller;
 			socklen_t size= sizeof(caller);
 			int *sock2= (int *) malloc(sizeof(int));
 			if(*sock2 >= 0){
 				printf("Nouvelle connexion.");
 				pthread_t th;

 				struct arg_struct *args= (struct arg_struct *)malloc(sizeof(arg_struct));
 				args->arg1= sock2;
 				args->arg2= annuaire;
 				args->arg3= taille;

 				if(pthread_create(&th, NULL, communication, (void*)args) != 0){
 					printf("Probleme de creation de thread");
 				}
 		    }
 		}
 	}
}