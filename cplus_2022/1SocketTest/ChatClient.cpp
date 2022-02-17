/*
Maker : GazerJ
Date: 2021.12.30
What:Chat app in Client Computer
How: TCP
Way: WinSock Init=>Arguments Init=>Make Sock=>Connect=>Received&Send=>Close 
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
extern void sendClient(int * );
extern void sendClient(int * ,struct sockaddr_in * );
char sendbuf[SEND_BUF_SIZE]= {0};
char recvbuf[SEND_BUF_SIZE]= {0};
int main(){
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
		printf("Failed to load Winsock.\n"); //Winsock ≥ı ºªØ¥ÌŒÛ
		return -1;
	}
	// Socket 
	int sock;
	struct sockaddr_in hostaddr,clientaddr;
	// Winsock Init
	int socklen=sizeof(clientaddr);
	memset((void *)&hostaddr,0,sizeof(hostaddr));
	hostAddrInit(&hostaddr);
	sock=socket(AF_INET,SOCK_STREAM,0);//getSocketFather
	connect(sock,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//connect
	sendClient(&sock,&hostaddr);
	close(sock);
}

void hostAddrInit(sockaddr_in*  hostaddr){
	(*hostaddr).sin_family=AF_INET;
	(*hostaddr).sin_port=htons(LISTEN_PORT);
	(*hostaddr).sin_addr.s_addr=inet_addr("192.168.0.108");
}

void sendClient(int * sock,struct sockaddr_in * hostaddr){
	while(1){
		printf("input something to send:\t");
		scanf("%s",sendbuf);
		send(*sock,sendbuf,sizeof(sendbuf),0);
		for(int i=0;i<SEND_BUF_SIZE;i++){sendbuf[i]='\0';}	
	}
	//printf("1234567");
	//sendClient(sock,hostaddr);
}
