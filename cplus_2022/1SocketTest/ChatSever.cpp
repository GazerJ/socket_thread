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
extern void waitForLink(int* ,int* ,sockaddr_in* ,int* );
char sendbuf[SEND_BUF_SIZE]= {0};
char recvbuf[SEND_BUF_SIZE]= {0};
int main(){
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
		printf("Failed to load Winsock.\n"); //Winsock ≥ı ºªØ¥ÌŒÛ
		return -1;
	}
	// Socket 
	int sockFather,sock;
	struct sockaddr_in hostaddr,clientaddr;
	// Winsock Init
	int socklen=sizeof(clientaddr);
	memset((void *)&hostaddr,0,sizeof(hostaddr));
	memset((void *)&clientaddr,0,sizeof(clientaddr));
	hostAddrInit(&hostaddr);
	sockFather=socket(AF_INET,SOCK_STREAM,0);//getSocketFather
	bind(sockFather,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//bind port
	listen(sockFather,MAX_LISTEN_NUM);//listen
	printf("wait for linking\n");
	
	waitForLink(&sock,&sockFather,&clientaddr,&socklen);// 
	close(sock);
	close(sockFather);
} 

void hostAddrInit(sockaddr_in*  hostaddr){
	(*hostaddr).sin_family=AF_INET;
	(*hostaddr).sin_port=htons(LISTEN_PORT);
	(*hostaddr).sin_addr.s_addr=htonl(INADDR_ANY);
}

void waitForLink(int* sock,int* socketFater,sockaddr_in* clientaddr,int* socklen){
	char *ptr;
	ptr=recvbuf;
	int leftlen=RECV_BUF_SIZE-1;
	*sock=accept((*socketFater),(struct sockaddr*)clientaddr,(int *)socklen);
	
	recv(*sock,ptr,leftlen,0);
	printf("[%s]: %s\n",inet_ntoa((*clientaddr).sin_addr),recvbuf);
	send(*sock,ptr,leftlen,0);
	for(int i=0;i<RECV_BUF_SIZE;i++)recvbuf[i]='\0';
	waitForLink(sock,socketFater,clientaddr,socklen);		
	
}
