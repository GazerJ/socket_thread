/*
Maker : GazerJ
Date: 2021.12.30
What:Chat app in Client Computer
How: TCP
Way: Init=>Make Sock=>Connect=>Received&Send=>Close 
*/
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <cstring>
#include <fstream>
#include <string>
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
/*
 *  
 * Client :TCP ; the  start function to go；
 * clientUDP:UDP ; the start function to go;
 *
 *
 *
 *
 *
 *  class : Client    :  TCP
 *  	main var : 
 *  		sock:  client  socket
 *  		flag:  the flag of connect
 *  		mode:  the mode of TCP or UDP
 *  		
 *	main func :
 *		makeSock()  : host init and manke socket
 *		Connect()   :  connet host
 *		sentdata(char *): send data to host ( both TCP and UDP ) 		
 *		start() :   the function to go
 *  
 *
 *
 *	
 *  class : clentUDP  extend Client :UDP
 *	different with Clinet:
 *		start() :   In UDP  ,Don`t Need Connect  ,Just Sendto The Port  
 *		makeSock():  there is SOCK_DGRAM mode
 *
 *
 *
 *
 *
 * 
 *
 */
class Client{
	public:
	int sock;//send socket
	int flag;// the flag of connect : #TCP  {-1 : "not connect" ; 0 :" connect" } #UDP:no connect flag=0；
	int mode;//the mode of Client  {  0 : "TCP";  1:" UDP" }
	struct sockaddr_in hostaddr;  // hostaddr 
        const int socklen=sizeof(hostaddr);

	void makeSock(){
		memset((void *)&hostaddr,0,sizeof(hostaddr));
		hostAddrInit(&hostaddr);
		sock=socket(AF_INET,SOCK_STREAM,0);//getSocketFather

	
	}
	void senddata(char * data){
		if(mode==0)send(sock,data,strlen(data),0);
		else if(mode==1) sendto(sock,data,strlen(data),0,(struct sockaddr*)(&hostaddr),sizeof(struct sockaddr_in));


	}
	void Connect(){
		flag=connect(sock,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//connect
		cout<<"flag:"<<flag<<endl;
	}
	Client(){
		sock=1;
		flag=-1;	
		mode=0;
		//makeSock();
		//Connect();
		//senddata((char *) "hello world ~");
	}

	void start(){
		makeSock();
		Connect();
		senddata((char *) "hello world ~");
	}




	void hostAddrInit(sockaddr_in*  hostaddr){
        	(*hostaddr).sin_family=AF_INET;
        	(*hostaddr).sin_port=htons(LISTEN_PORT);
        	(*hostaddr).sin_addr.s_addr=inet_addr("127.0.0.1");
	}

};


class clientUDP : public  Client{ 
	public:
		clientUDP(){
			sock=1;
			flag=0;
			mode=1;
		};
		void makeSock(){
			memset((void *)&hostaddr,0,sizeof(hostaddr));
			hostAddrInit(&hostaddr);
			sock=socket(AF_INET,SOCK_DGRAM,0);//getSocketFather
		};
		void start(){
			makeSock();
			senddata((char *)"hello world ~");
		};

};


class clientFile :public Client{
	public:
		clientFile(){
			sock=1;
			flag=-1;
			mode=0;
		};

		void transFile(  char  * name){
			start();
			senddata(name);
			sleep(0.001);
			ifstream inputfile(name);
			string  data;
			while (inputfile >>  data) {
				send(sock,(data).c_str(),data.size(),0);
				sleep(0.001);
			};

		};



};

int main(){
	//Client C1;
	/*
	clientUDP C1;
	C1.start();
	while(1){
		if(C1.flag==0){
			cout<<"[input something]:" ;
			for(int i=0;i<SEND_BUF_SIZE;i++)sendbuf[i]=0;
			cin.getline(sendbuf,SEND_BUF_SIZE);
			cout<<"have send: "<<sendbuf<<endl;
			C1.senddata(sendbuf);
		}
		else{ //TCP need connect  UDP don`t need
			cout<<"connecting ... "<<endl;
			C1.Connect();
		
		}
	}
	*/

	clientFile C1;
	char * s;
	cout<<"input file [ example :   ./client/1.txt]"<<endl;
	cin >>s;
	C1.transFile(s);

}
