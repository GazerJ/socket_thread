/*
Maker : GazerJ
Date: 2021.12.30
What: Chat app in Sever Computer
How: TCP
Way: WinSock Init=>Arguments Init=>Make Sock=>Bind Port=>Listen=>Accpet=>Received&Send=>Close 
*/
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <string.h>
#include <iostream>
//#include <sys/socket.h>
#include <Winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_LISTEN_NUM 5
#define SEND_BUF_SIZE 100
#define RECV_BUF_SIZE 100
#define LISTEN_PORT 7800
using namespace std;
extern void hostAddrInit(sockaddr_in* );
extern void waitForLink(int* ,sockaddr_in* ,int* );
extern void *chat(void *);
char sendbuf[SEND_BUF_SIZE]= {0};
char recvbuf[SEND_BUF_SIZE]= {0};
struct sockaddr_in hostaddr,clientaddr;
int main(){
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
		printf("Failed to load Winsock.\n"); //Winsock ≥ı ºªØ¥ÌŒÛ
		return -1;
	}
	// Socket 
	int sockFather;
	// Winsock Init
	int socklen=sizeof(clientaddr);
	memset((void *)&hostaddr,0,sizeof(hostaddr));
	memset((void *)&clientaddr,0,sizeof(clientaddr));
	hostAddrInit(&hostaddr);
	sockFather=socket(AF_INET,SOCK_STREAM,0);//getSocketFather
	bind(sockFather,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//bind port
	listen(sockFather,MAX_LISTEN_NUM);//listen
	printf("wait for linking\n");
	
	waitForLink(&sockFather,&clientaddr,&socklen);// 
	
	close(sockFather);
} 

void hostAddrInit(sockaddr_in*  hostaddr){
	(*hostaddr).sin_family=AF_INET;
	(*hostaddr).sin_port=htons(LISTEN_PORT);
	(*hostaddr).sin_addr.s_addr=htonl(INADDR_ANY);
}

void waitForLink(int* socketFater,sockaddr_in* clientaddr,int* socklen){
	
	int sock; 
	pthread_t tid;
	sock=accept((*socketFater),(struct sockaddr*)clientaddr,(int *)socklen);
	printf("welcome [%s]\n",inet_ntoa((*clientaddr).sin_addr));
	pthread_create(&tid,NULL,chat,(void *)&sock);	
	waitForLink(socketFater,clientaddr,socklen);		
}
void *chat(void *sendSocket){
	char sendbuf[SEND_BUF_SIZE]= {0};
	char recvbuf[SEND_BUF_SIZE]= {0};
	char *ptr;
	
	int leftlen=RECV_BUF_SIZE-1;
	int sock=*(int *)sendSocket; 
	while(1){
		ptr=recvbuf;
		recv(sock,ptr,leftlen,0);
		if (recvbuf[0]!='\0'){
			printf("[%s]: %s\n",inet_ntoa((clientaddr).sin_addr),recvbuf);
			//send(sock,ptr,leftlen,0);
			for(int i=0;i<RECV_BUF_SIZE;i++){
			recvbuf[i]='\0';}
		}
	}
	
}
