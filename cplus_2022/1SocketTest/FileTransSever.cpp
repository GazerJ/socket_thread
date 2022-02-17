/*
Maker : GazerJ
Date: 2021.12.30
What: FileTrans app in Sever Computer
How: TCP
Way: WinSock Init=>Arguments Init=>Make Sock=>Connect=>ReadFile&Send=>Close 
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
#include <pthread.h>
#define MAX_LISTEN_NUM 5
#define SEND_BUF_SIZE 1024
#define RECV_BUF_SIZE 1024
#define LISTEN_PORT 7800

using namespace std;
extern void hostAddrInit(sockaddr_in* );
extern void waitForLink(int* ,int* ,sockaddr_in* ,int* );
extern void *tranFile(void *sendSocket);
char sendbuf[SEND_BUF_SIZE]= {0};
char recvbuf[SEND_BUF_SIZE]= {0};
int main(){
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
		printf("Failed to load Winsock.\n"); //Winsock 初始化错误
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
	bind(sockFather,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//
	listen(sockFather,MAX_LISTEN_NUM);//listen
	printf("wait for linking\n");
	pthread_t tid;
	while(1){
		sock=accept(sockFather,(struct sockaddr *)&hostaddr,(int *)&socklen);
		printf("linked");
		pthread_create(&tid,NULL,tranFile,(void *)&sock);
	}
	close(sock);
	close(sockFather);
	
}

void hostAddrInit(sockaddr_in*  hostaddr){
	(*hostaddr).sin_family=AF_INET;
	(*hostaddr).sin_port=htons(LISTEN_PORT);
	(*hostaddr).sin_addr.s_addr=htonl(INADDR_ANY);
	
}
void *tranFile(void *sendSocket){
	int sock=*(int *)sendSocket;
	char buf[1024];
	int buflen=1024,k=0;
	char file[256]={0};
	sprintf(file,"1.txt");
	FILE *source;
	source = fopen(file,"rb");
	printf("begin tran file %s\n",file);
	//write(sock,file,strlen(file));//写文件名
	send(sock,file,strlen(file),0);//写文件名
	memset(buf,0,buflen);
	//read(sock,buf,1024);
	
	memset(buf,0,buflen);
	k=1;
	while(k>0){
		k=fread(buf,1,buflen,source);
		printf("tran file Length:%d\n",k);
		send(sock,buf,SEND_BUF_SIZE,0);
		memset(buf,0,buflen);	
	}
	fclose(source);
	close(sock); 
	
}
