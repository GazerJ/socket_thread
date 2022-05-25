/*
Maker : GazerJ
Date: 2021.12.30
What: Chat app in Sever Computer
How: TCP
Way:  Init=>Make Sock=>Bind Port=>Listen=>Accpet=>Received&Send=>Close 
*/
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <fstream>

#include <functional>
//#include<pthread.h>
#define MAX_LISTEN_NUM 5      //max num of listening
#define SEND_BUF_SIZE 100    //buffer size 
#define RECV_BUF_SIZE 100   //buffer size
#define LISTEN_PORT 7800   //port 

using namespace std;    


/*
 *  Class Server:
 *	main var:
 * 	
 * 		int sock          =>   the sock to work
 *		
 *		int sockFather     =>   the father socket token
 *	
 *		char sendbuf[SIZE]
 *	
 *		cahr recvbuf[SIZE]
 *	
 *	main function:
 *
 *		void waitForLink   wait for accept and show the data
 *
 *		hostAddrInit     => init the host addr
 *  
 *
 *
 *
 *
 *
 */



class Serve{	
		public:
		Serve(){};

		void start(){
			static int sockFather;
                        static struct sockaddr_in hostaddr,clientaddr;
                        static int len;
	
		        hostAddrInit(& (hostaddr));
                        sockFather=socket(AF_INET,SOCK_STREAM,0);//getSocketFather
                        bind(sockFather,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//bind port
                        listen(sockFather,MAX_LISTEN_NUM);//listen
                        printf("wait for linking\n");
                        len = sizeof(clientaddr);
                        waitForLink(&sockFather,&clientaddr,&len);

		};	

		void hostAddrInit(sockaddr_in*  hostaddr){  //init function
		        (*hostaddr).sin_family=AF_INET;
		        (*hostaddr).sin_port=htons(LISTEN_PORT);
        		(*hostaddr).sin_addr.s_addr=htonl(INADDR_ANY);
		};

		struct para{
			int * sock;
			sockaddr_in * clientaddr;
		};
		// auto sockHandle= [&](void* par) -> void {
		static void sockHandle( void * par){
                        int * sock=((( para *)par)->sock);
                        sockaddr_in* clientaddr=( ((para *)par)->clientaddr);
                        char *ptr;
			char recvbuf[SEND_BUF_SIZE]= {0};
			ptr=recvbuf;
                        int leftlen=RECV_BUF_SIZE-1;
                        int token=1;
                        while(1){
                                token=recv(*sock,ptr,leftlen,0);
                                if (token!=0){
                                cout<<"["<<(inet_ntoa((*clientaddr).sin_addr))<<"]:"<<recvbuf<<endl;
				for(int i=0;i<leftlen;i++)recvbuf[i]=0;
                                }
                                else{
                                        cout<<"["<<inet_ntoa((*clientaddr).sin_addr) <<"] Cut off "<<endl;
                                        break;
                                }


                        }


                };

		private:	
		void waitForLink(int* socketFater,sockaddr_in* clientaddr,int* socklen){
        		int sock;
			sock=accept((*socketFater),(struct sockaddr*)clientaddr,(socklen_t *)socklen);
			para par;
			par.sock=&sock;
			par.clientaddr=clientaddr;
			thread t1(sockHandle,(void *)&par); 

        		waitForLink(socketFater,clientaddr,socklen);
		};
	
};


class serveUDP: public Serve{
	public:
		void start(){
		
			static int sockFather;
                        static struct sockaddr_in hostaddr,clientaddr;
                        static int len;
                        hostAddrInit(& (hostaddr));
                        sockFather=socket(AF_INET,SOCK_DGRAM,0);//getSocketFather
                        bind(sockFather,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//bind port
                        listen(sockFather,MAX_LISTEN_NUM);//listen
                        printf("Waiting for data at Port %d\n",LISTEN_PORT);
                        waitForData(&sockFather);

		};
		void waitForData( int * sock){
                        char *ptr;
                        char recvbuf[SEND_BUF_SIZE]= {0};
                        ptr=recvbuf;
                        int leftlen=RECV_BUF_SIZE-1;
                        int token=1;
                        cout<<"start"<<endl;
			while(1){
                                token=recv(*sock,ptr,leftlen,0);
                                cout<<"["<<LISTEN_PORT<<"]:"<<recvbuf<<endl;


                        };


		}
};

class serveFile:public Serve{
	public :
               void start(){
                        static int sockFather;
                        static struct sockaddr_in hostaddr,clientaddr;
                        static int len;

                        hostAddrInit(& (hostaddr));
                        sockFather=socket(AF_INET,SOCK_STREAM,0);//getSocketFather
                        bind(sockFather,(struct sockaddr*)&hostaddr,sizeof(hostaddr));//bind port
                        listen(sockFather,MAX_LISTEN_NUM);//listen
                        printf("wait for linking\n");
                        len = sizeof(clientaddr);
                        waitForLink(&sockFather,&clientaddr,&len);

                };

		void waitForLink(int* socketFater,sockaddr_in* clientaddr,int* socklen){
			int sock;
                        sock=accept((*socketFater),(struct sockaddr*)clientaddr,(socklen_t *)socklen);
                        para par;
                        par.sock=&sock;
                        par.clientaddr=clientaddr;
                        thread t1(sockHandleFile,(void *)&par);
                        waitForLink(socketFater,clientaddr,socklen);
                
		};

		static void sockHandleFile( void * par){
		int * sock=((( para *)par)->sock);
                sockaddr_in* clientaddr=( ((para *)par)->clientaddr);
                char *ptr;
                char recvbuf[SEND_BUF_SIZE]= {0};
                ptr=recvbuf;
                int leftlen=RECV_BUF_SIZE-1;
                int token=1;
		//recv(*sock,ptr,leftlen,0); //hello world ~
		//recv(*sock,ptr,leftlen,0); //file name 
		ofstream file("./serve/recv.txt");
		for(int i=0;i<leftlen;i++)recvbuf[i]=0;
		while(1){
			token=recv(*sock,ptr,leftlen,0);
                        if (token!=0){
				cout<< "trans file";
                              	 cout<<"["<<(inet_ntoa((*clientaddr).sin_addr))<<"]:"<<recvbuf<<endl;
                       		 file<< recvbuf<<endl;
				 for(int i=0;i<leftlen;i++)recvbuf[i]=0;
                        }
                        else{
                                 cout<<"["<<inet_ntoa((*clientaddr).sin_addr) <<"] Trans File over "<<endl;
                                 break;
                                }


                        }


                };

 
 
};


int main(){
	cout<<"input mode {0: TCP ;1:UDP ; 2:TCP trans File}"<<endl;
	int m;
	cin>>m;
	
	if(m==1){serveUDP   S;S.start();}
	else if (m==0) {Serve S;S.start();}
	else if (m==2) {serveFile S; S.start();}
	return 0;
} 

