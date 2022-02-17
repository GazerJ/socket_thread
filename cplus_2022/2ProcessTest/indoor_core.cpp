
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAX_LISTEN_NUM 5
#define SEND_BUF_SIZE 100
#define RECV_BUF_SIZE 100
#define LISTEN_PORT 7800
#define FIFO "/tmp/myfifoindoor"
using namespace std;
extern void hostAddrInit(sockaddr_in* );
extern void FIFO_init(int* fd);
extern void waitForLink(int* ,int* ,sockaddr_in* ,unsigned int* ,int* fd);
char sendbuf[SEND_BUF_SIZE]= {0};
char recvbuf[SEND_BUF_SIZE]= {0};
int main(){
	// Socket 
	int sockFather,sock;
	int fd=-1;
	struct sockaddr_in hostaddr,clientaddr;
	printf("open FIFO\n");
	FIFO_init(&fd);
	printf("Wait for linking (FIFO)\n");
	unsigned int socklen=sizeof(clientaddr);
	memset((void *)&hostaddr,0,sizeof(hostaddr));
	memset((void *)&clientaddr,0,sizeof(clientaddr));
	hostAddrInit(&hostaddr);
	sockFather=socket(AF_INET,SOCK_STREAM,0);//getSocketFather
	bind(sockFather,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//bind port
	listen(sockFather,MAX_LISTEN_NUM);//listen
	printf("wait for linking\n");
	sock=accept((sockFather),(struct sockaddr*)&clientaddr,&socklen);
	waitForLink(&sock,&sockFather,&clientaddr,&socklen,&fd);// 
	close(sock);
	close(sockFather);
} 

void hostAddrInit(sockaddr_in*  hostaddr){
	(*hostaddr).sin_family=AF_INET;
	(*hostaddr).sin_port=htons(LISTEN_PORT);
	(*hostaddr).sin_addr.s_addr=htonl(INADDR_ANY);
}

void waitForLink(int* sock,int* socketFater,sockaddr_in* clientaddr,unsigned int* socklen,int* fd){
	char *ptr;
	
	int leftlen=RECV_BUF_SIZE;
	while(1){

		ptr=recvbuf;
		recv(*sock,ptr,leftlen,0);
		if(recvbuf[0]==0)return;
		printf("[%s]: %s\n",inet_ntoa((*clientaddr).sin_addr),recvbuf+1);
		
		memset(recvbuf,0,RECV_BUF_SIZE);
		int len=0;
		len=read(*fd,recvbuf,SEND_BUF_SIZE);
		if (len>0){
			send(*sock,recvbuf,leftlen,0);
		}
		//send(*sock,ptr,leftlen,0);
	}
	
}
void FIFO_init(int* fd){
	if (access(FIFO,F_OK)!=0){
		if(mkfifo(FIFO,0666)<0){
			perror("cannot creat fifo");
		}
	}
	*fd=open(FIFO,O_RDONLY,0);
	printf("FIFO ready\n");
}
