#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<time.h>

int main(int argc, char const *argv[]){
	int ClientSocket,i;
	struct sockaddr_in ServerAddr;
	char *msgS=(char *)malloc(2048);
	char *msgR=(char *)malloc(2048);
	char *id=(char *)malloc(2048);
	char *ident=(char *)malloc(2048);

	printf("Saisie votre id:");
	scanf("%s",id);
	fd_set readFD,readAllFD;

	ClientSocket=socket(AF_INET,SOCK_STREAM,0);

	ServerAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port=htons(8000);
	memset(&(ServerAddr.sin_zero),'\0',8);

	connect(ClientSocket,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr));

	FD_ZERO(&readAllFD);
	FD_ZERO(&readFD);
	FD_SET(0,&readAllFD);
	FD_SET(ClientSocket,&readAllFD);

	while(1){
		memset(ident,'\0',sizeof(ident));
		strcat(ident,id);
		strcat(ident," : ");
		readFD=readAllFD;
		select(ClientSocket+1,&readFD,NULL,NULL,NULL);
		for(i=0;i<=ClientSocket+1;i++){
				if(FD_ISSET(i,&readFD)){
					if(i==0){
						scanf("%s",msgS);
						strcat(ident,msgS);
						send(ClientSocket,(void *)ident,1024,0);
					}else{
						int n=recv(ClientSocket,msgR,1024,0);
						puts((char *)msgR);
					}
				}
		}
	}
	return 0;
}
