#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>


void list();
void msg(int sock);
void last(int sock);
char id_client [8];
    
int main(int argc, char** argv) {
    if(argc < 2){
    	perror("La ligne de commande doit être de la forme \"java Client1 id_client\".\n");
    	exit(EXIT_FAILURE);
    }
    strcpy(id_client, argv[1]); /*copie de l'id du client*/
    /*ajout des dieses*/
    int len= 8 - strlen(id_client);
    char diese[len+1];
    memset(diese, '#', len);
    diese[len]= '\0';
    strcat(id_client, diese);
    printf("Bienvenue maitre %s.\n", id_client);

    while(1){
    	int res;
    	char* cmd= NULL;
    	size_t ta= 0;
    	getline(&cmd, &ta, stdin);
    	cmd= strtok(cmd, "\n");
    	if(strcmp(cmd, "LIST") == 0){
    		list();
    	}else if(strcmp(cmd, "LAST") == 0 || strcmp(cmd, "MESS") == 0) {
    		struct sockaddr_in adresse;
    		char* adr= NULL;
    		size_t taille= 0;
    		adresse.sin_family= AF_INET;
    		
    		/*printf("ADRESSE IP: ");
    		getline(&adr, &taille, stdin);*/
    		printf("PORT: ");
    		int port;
    		scanf("%d", &port);
    		
    		adresse.sin_port= htons(port);
    		inet_aton("172.20.10.6", &adresse.sin_addr);
    		int sock = socket(PF_INET, SOCK_STREAM, 0);
    	    int r= connect(sock, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in));
    		
    		
    		if(r == -1){
    			perror("erreur connect\n");
    			exit(EXIT_FAILURE);
    		}

    		
    		if(cmd[0] == 'M'){
    			msg(sock);
    		}else{
    			last(sock);
    		}
    		close(sock);
    		free(cmd);

    	}else{
    	    printf("Les seules commandes reconnues sont :\n\"MESS\" - envoyer un message à diffuser\n\"LIST\" - demander au gestionnaire la liste des diffuseurs\n\"LAST\" - demander à voir les derniers messages\n");         
    	}
    }
    return 0;
}

void list(){
	struct sockaddr_in adresse;
	adresse.sin_family= AF_INET;
	char adr[33];
	write(1, "ADRESSE IP:  ", 13);
    int r= read(0, adr, 33);
    adr[r-1]= '\0';
    write(1, "PORT: ", 6);
    char tmp[5]; 
    r= read(0, tmp, 5);
    adr[r-1]= '\0';
    int port= atoi(tmp);
    adresse.sin_port= htons(port);
    inet_aton(adr, &adresse.sin_addr);

    int sock= socket(PF_INET, SOCK_STREAM, 0);
   	r= connect(sock, (struct sockaddr *)&adresse, sizeof(struct sockaddr));
    if(r == -1){
    	perror("erreur connect");
    	exit(EXIT_FAILURE);
    }
   send(sock, "LIST\r\n", 6, 0);
   char rec[56];
   r= recv(sock, rec, 56, 0);
   rec[r-1]= '\0';
   char maxStr[4];
   strncat(maxStr, rec, 2);
   int max= atoi(maxStr);
   for(int i= 0; i< max; i++){
   	  r= recv(sock, rec, 55, 0);
   	  write(1, rec, r);
   }
   close(sock);
}

void last(int sock){
	char sent[11];
	char *msg= "ENTRE 0 ET 999, NOMBRE DE MESSAGES À AFFICHER: ";
	write(1, msg, strlen(msg));
	int res;
	char tmp[5]; 
	int r= read(0, tmp, 4);
	tmp[r]= '\0';
	res= atoi(tmp);
	while(res < 0 | res > 999){
		write(1, msg, strlen(msg));
		read(0, tmp, 4);
		tmp[r]= '\0';
		res= atoi(tmp);
	}
	sprintf(sent, "LAST %d\r\n", res);
	send(sock, sent, strlen(sent), 0);
	char rec[162];
	r= recv(sock, rec, 161, 0);
	rec[r]= '\0';
	write(1, rec, r);
	while(strcmp("ENDM\r\n", rec) != 0){
		r= recv(sock, rec, 161, 0);
		rec[r]= '\0';
		write(1, rec, r);
	}
}

void msg(int sock){
	char* input= NULL;
	size_t taille = 0; 
	
	printf("Entrez le message à envoyer au diffuseur:\n");
	getline(&input, &taille, stdin);
	input= strtok(input, "\n");

	printf("You typed: '%s'\n", input);	
	char msg[157];
	sprintf(msg, "MESS %s %s ", id_client, input);
	msg[strlen(msg)-1]= '\0';
	printf("%s\n", msg);

	int len= 140 - strlen(input);
	char diese[len];
	memset(diese, '#', len-2);
	diese[len-1]= '\0';
	strcat(msg, diese);
	strcat(msg, "\r\n\0");
	printf("2: %s", msg);
	send(sock, msg, 156, 0);

	char rec[7];
	int r= recv(sock, rec, 6, 0);
	rec[r]= '\0';
	if(strcmp(rec, "ACKM\r\n") == 0){
		write(1, rec, r);
	}else{
		perror("Il y'a eu une erreur, recommencez.");
		exit(EXIT_FAILURE);
	}
	free(input);
}