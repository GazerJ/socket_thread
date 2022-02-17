#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define BUF_MAX_LEN 64
//有名管道的标识符
#define FIFO "/tmp/myfifoindoor"
int main(int argc,char **argv){
	int fd = -1;
	char buf[BUF_MAX_LEN] = {0};
	
	// 打开FIFO管道 设置非阻塞标识
	fd=open(FIFO,O_WRONLY|O_NONBLOCK,0);
	while(1){
		printf("send:");
		scanf("%s",buf);
		write(fd,buf,BUF_MAX_LEN);
		memset(buf,0,BUF_MAX_LEN);

	}
	close(fd);
	return 0;
}

