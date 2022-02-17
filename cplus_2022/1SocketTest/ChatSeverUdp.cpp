/*
Maker : GazerJ
Date: 2021.12.31
What: Chat app in Sever Computer
How: UDP
Way: WinSock Init=>Arguments Init=>Make Sock=>Bind=>Receive=>Close 
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
extern void getdata(int *);
char sendbuf[SEND_BUF_SIZE]= {0};
char recvbuf[SEND_BUF_SIZE]= {0};
int main(){
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
		printf("Failed to load Winsock.\n"); //Winsock ≥ı ºªØ¥ÌŒÛ
		return -1;
	}
	// Sock
	int sock;
	struct sockaddr_in hostaddr,clientaddr;
	int socklen=sizeof(clientaddr);
	memset((void *)&hostaddr,0,sizeof(hostaddr));
	hostAddrInit(&hostaddr);
	sock=socket(AF_INET,SOCK_DGRAM,0);
	bind(sock,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//bind
	printf("waiting for infomation");
	getdata(&sock); 
	close(sock); 
} 
void hostAddrInit(sockaddr_in*  hostaddr){
	(*hostaddr).sin_family=AF_INET;
	(*hostaddr).sin_port=htons(LISTEN_PORT);
	(*hostaddr).sin_addr.s_addr=htonl(INADDR_ANY);
	//(*hostaddr).sin_addr.s_addr=inet_addr("127.0.0.1");
}
void getdata(int *sock){
	recv(*sock,recvbuf,RECV_BUF_SIZE,0);
	printf("\n[someone UDP]:%s",recvbuf); 
	getdata(sock);
}
