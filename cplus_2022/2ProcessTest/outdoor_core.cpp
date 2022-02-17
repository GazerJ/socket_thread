
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
#define FIFO "/tmp/myfifooutdoor"
using namespace std;
extern void hostAddrInit(sockaddr_in* );
extern void FIFO_init(int* fd);
extern void waitForLink(int* ,sockaddr_in* ,unsigned int* ,int* fd);
char sendbuf[SEND_BUF_SIZE]= {0};
char recvbuf[SEND_BUF_SIZE]= {0};
int main(){
	// Socket 
	int sockFather;
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
	connect((sockFather),(struct sockaddr*)&hostaddr,sizeof(hostaddr));
	waitForLink(&sockFather,&clientaddr,&socklen,&fd);// 
	
	close(sockFather);
} 

void hostAddrInit(sockaddr_in*  hostaddr){
	(*hostaddr).sin_family=AF_INET;
	(*hostaddr).sin_port=htons(LISTEN_PORT);
	(*hostaddr).sin_addr.s_addr=inet_addr("127.0.0.1");
}

void waitForLink(int* sock,sockaddr_in* clientaddr,unsigned int* socklen,int* fd){
	char *ptr;
	int leftlen=RECV_BUF_SIZE;
	while(1){
		sleep(1);
		int len=0;
		len=read(*fd,recvbuf,SEND_BUF_SIZE);
		if (len>0){
			if(recvbuf[0]==0)return;
			send(*sock,recvbuf,leftlen,0);
		}

		ptr=recvbuf;
		recv(*sock,ptr,leftlen,0);
		printf("[indoor]: %s\n",recvbuf+1);
		memset(recvbuf,0,RECV_BUF_SIZE);

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
