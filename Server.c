#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<time.h>


int main(int argc, char const *argv[]){
    
    int ServerSocket,ClientSocket,len,taille,Client[100],count,i,MaxFD;
    struct sockaddr_in ServerAddress,ClientAddress;
    char * msg=(char *)malloc(2048);
    fd_set readAllFD,readFD;

    taille=sizeof(msg);
    count=0;

    ServerSocket=socket(AF_INET,SOCK_STREAM,0);

    ServerAddress.sin_addr.s_addr=htonl(INADDR_ANY);
    ServerAddress.sin_port=htons(8000);
    ServerAddress.sin_family=AF_INET;

    len=sizeof(ServerAddress);

    bind(ServerSocket,(struct sockaddr *)&ServerAddress,len);

    listen(ServerSocket,5);

    MaxFD=ServerSocket;
    FD_ZERO(&readAllFD);
    FD_SET(ServerSocket,&readAllFD);
    for(;;){
        FD_ZERO(&readFD);
        readFD=readAllFD;
        select(MaxFD+1,&readFD,NULL,NULL,NULL);
        if(FD_ISSET(ServerSocket,&readFD)){
            ClientSocket=accept(ServerSocket,(struct sockaddr *)&ClientAddress,&len);
            FD_SET(ClientSocket,&readAllFD);
            Client[count++]=ClientSocket;
            MaxFD=(ClientSocket>MaxFD?ClientSocket:MaxFD);
			printf("nouveau client communicant via DS , %d\n",ClientSocket);
        }else
            for(i=0;i<count;i++){
                ClientSocket=Client[i];
                if(FD_ISSET(ClientSocket,&readFD)){
                    memset(msg,'\0',taille);
                    int n=recv(ClientSocket,(void *)msg,1024,0);
                    if(n>0){

                        for(i=0;i<count;i++){
                            if(Client[i]!=ClientSocket){
                                send(Client[i],(void *)msg,1024,0);
                            }
                        }
                    }else if(n==0){
                        // supprimer le client de la liste des clients 
                        FD_CLR(ClientSocket,&readAllFD);
                        int j;
                        for(j=i;j<count-1;j++) Client[j]=Client[j+1];
                        count--;
                        if((ClientSocket==MaxFD) && count!=0)
                            MaxFD =0;
                            for(j=1;j<count;j++)
                                if(Client[j]>MaxFD) MaxFD=Client[j];
                        memset(msg,'\0',2048);
                        printf("le client communicant via DS ,s'est deconnecte %d\n",ClientSocket);
                        break;
				}
                }
            }
    }
    return 0;
}