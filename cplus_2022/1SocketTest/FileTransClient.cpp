/*
Maker : GazerJ
Date: 2021.12.31
What: FileTrans app in Client Computer
How: TCP
Way: WinSock Init=>Arguments Init=>Make Sock=>Connect=>Receive&WriteFile=>Close
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
extern void writeFile(int * sock,FILE * target);
char sendbuf[SEND_BUF_SIZE]= {0};
char recvbuf[SEND_BUF_SIZE]= {0};
char file[RECV_BUF_SIZE]={0};
char buf[RECV_BUF_SIZE]={0};
int main() {
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
		printf("Failed to load Winsock.\n"); //Winsock ≥ı ºªØ¥ÌŒÛ
		return -1;
	}
	int sock;
	struct sockaddr_in hostAddr;
	memset(&hostAddr,0,sizeof(hostAddr));
	hostAddrInit( &hostAddr);
	sock=socket(AF_INET,SOCK_STREAM,0);
	cout<<connect(sock,(struct sockaddr*)&hostAddr,sizeof(hostAddr));
	int receivelength=0;
	//while(receivelength<=0) {receivelength=read(sock,file ,sizeof(file));}
	receivelength=recv(sock,file ,sizeof(file),0);
	printf("receive name length:%d\n",receivelength);
	FILE *target;
	sprintf(buf,"%s_%s","receive",file);
	printf("begin receive file %s\n",file);
	target=fopen(buf,"wb");
	sprintf(buf,"read");
	//write(sock,buf,strlen(buf));
	memset(buf,0,RECV_BUF_SIZE);
	writeFile(&sock,target);
	fclose(target);
	close(sock);
	printf("[System]:all write over");
}

void hostAddrInit(sockaddr_in*  hostaddr) {
	(*hostaddr).sin_family = AF_INET;
	(*hostaddr).sin_port = htons(LISTEN_PORT);
	(*hostaddr).sin_addr.s_addr = inet_addr("127.0.0.1");

}
void writeFile(int * sock,FILE * target){
	int receivelength;
	receivelength=recv(*sock,buf,SEND_BUF_SIZE,0);
	if(buf[0]=='\0')return;
	printf("write :\n %s\n",buf);
	fwrite(buf,receivelength,1,target);
	memset(buf,0,RECV_BUF_SIZE);
	writeFile(sock,target);
}
















