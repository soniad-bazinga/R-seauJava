#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void list();
void msg(int* sock);
void last(int* sock);
char id_client [8];


int main(int argv, const char** argc) {
    if(argv < 2){
        printf("La ligne de commande doit être de la forme \"./client id_client\".\n");
    }else {
        strcpy(id_client, argc[1]); //ajouter les dièses
        int len = 8-strlen(id_client);
        char diese[len+1];
        memset(diese,'#',len);
        diese[len]='\0';
        strcat(id_client, diese);
        int res;
	    while (1){
            char* cmd= (char*) malloc(5*sizeof(char));
            if(!cmd){
                perror("erreur malloc");
                exit(EXIT_FAILURE);
            }
            memset(cmd, '\0', 5);
            res= read(STDIN_FILENO, cmd, 5);
            if (res == -1){
                perror("erreur read");
                exit(EXIT_FAILURE);
            }
            cmd[4]='\0';
            //write(1, cmd, strlen(cmd));
            if(strcmp(cmd, "LIST")==0){
		        list();
		    }else if (strcmp(cmd, "LAST")== 0|| strcmp(cmd, "MESS")== 0){
			    int* sock = malloc(sizeof(int));
                *sock= socket(PF_INET, SOCK_STREAM, 0);
                struct sockaddr_in adresse;
                adresse.sin_family = AF_INET;
                char ip[16];
                memset(ip, '\0', 16);
                printf("Choisissez l'adresse ip du diffuseur avec lequel vous souhaitez communiquer.\n");
                scanf("%s",ip);
                while (strlen(ip)!=15){
                    printf("L'adresse ip doit être de longueur 15 ! Recommencez.\n");
                    scanf("%s",ip);
                }
                int port1 = 0;
                printf("Sur quel port tourne le diffuseur que vous souhaitez écouter ?\n");
                scanf("%d",&port1);
                while (port1 < 1000 || port1 >9999){
                    printf("%d\n",port1);
                    printf("Le numéro de port doit contenir quatre chiffres ! Recommencez.\n");
                    scanf("%d",&port1);
                }
                adresse.sin_port = htons(port1);
                inet_aton("localhost", &adresse.sin_addr);
                int r = connect(*sock, (struct sockaddr *) &adresse, sizeof (struct sockaddr_in));
                if (r!=-1){
                    printf("HIIIII");
		            if (cmd[0]=='M'){
				        msg(sock);
                    }else {
				        last(sock);
                    }
                }else{
                    perror("erreur connect");
                    exit(EXIT_FAILURE);
                }
                free(sock);

            }else
		        printf("Les seules commandes reconnues sont :\n\"MESS\" - envoyer un message à diffuser\n\"LIST\" - demander au gestionnaire la liste des diffuseurs\n\"LAST\" - demander à voir les derniers messages\n"); 
            free(cmd);
        }
    }
}

void list(){
    struct sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(5050);
    inet_aton("127.0.0.1", &adresse.sin_addr);
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock!=-1)
    {
        int r = connect(sock, (struct sockaddr *) &adresse, sizeof (adresse));
        if(r != -1){
            char* sent = "LIST\r\n";
            send(sock, sent, strlen(sent)+1, 0);
            //affichage de la réponse du diffuseur
            char rec[57];
            memset(rec, '\0', 57);
            r = recv (sock, rec, 9, 0);
            printf("%s",rec);
            //VERIFIER LE FORMAT DE NUM
            char maxStr[3];
            memset (maxStr, '\0', 4);
            strncat(maxStr, &rec[5], 2);
            int max = atoi ((maxStr));
            for (int i=0; i < max; i++) {
                memset(rec, '\0', 57);
                r = recv (sock, rec, 57, 0);
                write(1,rec, r);
            }
            close(sock);
        }
    }
    else 
        printf("else\n");
 }

 void last(int* sock){
    char sent[11];
    strcpy(sent, "LAST ");
    printf("Combien de messages souhaitez-vous voir ?\n");
    int res;
    scanf("%d", &res);
    while(res<0 || res>999){
        printf("Combien de messages souhaitez-vous voir ?\n");
        scanf("%d", &res);
    }
    char resStr[4];
    sprintf(resStr, "%d", res);
    resStr[3]='\0';
    strcat(sent, resStr);
    strcat(sent, "\r\n");
    sent[10]='\0';

    send(*sock, sent, strlen(sent), 0);
	char rec[161];
    memset(rec, '\0', 161);
    int r = recv(*sock, rec, 161, 0);
	write(1, rec, r);
    while(strcmp("ENDM\r\n", rec)){
        memset(rec, '\0', 161);
        r = recv(*sock, rec, 161, 0);
	    write(1, rec, r);
    }
	close(*sock);
 }

 void msg(int *sock){
    printf("Entrez le message à envoyer au diffuseur.\n");
    char input[140];
    memset(input, '\0', 140);
    //char* test = fgets (input, 140, stdin);
    int r = read(0, input, 140);
    if (r == -1) printf("oupsie erreur\n");
    input[r-1]='\0';
    char msg[157];
    memset(msg, '\0', 157);
    strcpy(msg, "MESS ");
    strcat(msg, id_client);
    strcat(msg, " ");
    strcat(msg, input);

    int len = 140 - strlen(input);
    char diese[len];
    memset(diese, '\0', len);
    for (int i =0; i < len; i++)
        diese[i] = '#';
    strcat(msg, diese);
    strcat(msg,"\r\n\0");
    send(*sock, msg, 157, 0);
    char rec[7];
    r = recv(*sock, rec, 7, 0);
    rec[r]= '\0';
    if (strcmp(rec,"ACKM\r\n")!=0)
        printf("Il y a eu une erreur, recommencez.");
    else 
        printf("%s", rec);
    close(*sock);
 }
