#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>
#include<unistd.h>

#define ID "CLIENT1#"
#define MESSAGE "Seek success, but always be prepared for random cats."

/*ce client doit pouvoir recevoir la diffusion des messages et aussi envoyer en 
mode TCP un message MESS au diffuseur pour qu'il diffuse son propre message. 
MULTICAST + TCP
*/

int main(int argc, char** argv){
	if (argc != 3){
    	printf("Il faut fournir un numéro de port\n");
    	return 0;
  	}
  int port= atoi(argv[1]);  /*port de communication TCP*/
  struct sockaddr_in adress_sock;
  adress_sock.sin_family= AF_INET;
  adress_sock.sin_port= htons(port);
  inet_aton("127.0.0.1",&adress_sock.sin_addr);

  int socket_in= socket(PF_INET, SOCK_STREAM, 0);
  int connect_in= connect(socket_in, (struct sockaddr*)&adress_sock,
                          sizeof(struct sockaddr_in));


  int sock_diff= socket(PF_INET, SOCK_DGRAM, 0);
  int ok= 1;
  int r = setsockopt(sock_diff, SOL_SOCKET, SO_REUSEPORT, &ok, sizeof(ok));
  struct sockaddr_in adress_diff;
  adress_diff.sin_family= AF_INET;
  adress_sock.sin_port= htons(atoi(argv[2]));  /*port d'écoute de la diffusion*/
  adress_sock.sin_addr.s_addr= htonl(INADDR_ANY);
  r= bind(sock_diff, (struct sockaddr*)&adress_sock, sizeof(struct sockaddr_in));
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr("127.0.0.1");
  mreq.imr_interface.s_addr= htonl(INADDR_ANY);
  r= setsockopt(sock_diff, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
  char tampon[200];


  if(connect_in == 0){
  	char buff[153];
  	int k= sprintf(buff, "MESS %s %s\n", ID, MESSAGE);
  	while(1){
  		int rec= recv(sock_diff, tampon, 200, 0);
  		tampon[rec]= '\0';
  		write(0, tampon, rec);

  		char str;
  		read(1, &str, 1);
  		if(str == 'M'){
  			/*envoyer le message en TCP*/
  			if(send(socket_in, buff, strlen(buff)*sizeof(char), 0) == -1){
       			perror("erreur send");
        		exit(EXIT_FAILURE);
      		}
  		}
  	}

  }else{
  	printf("UH OH!");
  }
}