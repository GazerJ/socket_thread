/*
Maker : GazerJ
Date: 2021.12.30
What: Download app in Sever/Client Computer
How: ***
Way: Init=>*=>*=>*=>*=>*=>*=>Close
*/
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#define MAX_URL_LEN (200)
#define MAX_PATH_LEN (100)
#define MAX_TASK_CNT (10)

typedef struct{
	void** items;
	int item_maxcount;
	int item_count;
	int item_size;
}__Z_SLIST;

typedef void* Z_SLIST;

typedef struct _DOWNLOAD_TASK{
	char url[MAX_URL_LEN];
	char save_path[MAX_PATH_LEN];
}__download_task;


static Z_SLIST task_slist=NULL;

pthread_t tid=0;
extern Z_SLIST task_slist_new(int,int);
extern int task_slist_delete(Z_SLIST,int);
extern int task_slist_add(Z_SLIST,const void *);
extern void* task_slist_add(Z_SLIST,int );
extern int task_add(char *,char*);
extern void *download_thread(void *arg);
extern int init_service(void);
extern int deinit_service(void);

int task_slist_delete(Z_SLIST list,int idx){
	__Z_SLIST* l=(__Z_SLIST*)list;
	if(idx>=0 && idx<l->item_count){
		int i=0;
		for(i=idx;i<l->item_count-1;i++){
			memcpy((char*)l->items+i*l->item_size,(char*)l->items+(i+1)*l->item_size,(int)l->item_size); 
		}
	}	l->item_count--;
}

void* task_slist_get(Z_SLIST list,int idx){
	__Z_SLIST* l=(__Z_SLIST*)list;
	if(!(idx>=0 && idx<l->item_count )){
		return NULL;
	}
	return (void *)(char *)l->items+idx*l->item_size;
}


int task_slist_add(Z_SLIST list,const void * item){
	__Z_SLIST* l=(__Z_SLIST*)list;
	if(l->item_count>=l->item_maxcount){
		return -1;
	}
	if(l->item_count<l->item_maxcount){
		memcpy((char*)l->items+l->item_count*l->item_size,item,(int)l->item_size);
		l->item_count++;
	}
	return 0;
}

int task_add(char* url ,char* save_path){
	int ret =-1;
	__download_task task={0};
	strncpy(task.url,url,sizeof(task.url));
	strncpy(task.save_path,save_path,sizeof(task.save_path));
	return task_slist_add(task_slist,&task);
}

Z_SLIST task_slist_new(int item_size,int item_maxcount){
	__Z_SLIST* list=NULL;
	int itemstotalsize=item_size * item_maxcount;
	list=(__Z_SLIST*)malloc(sizeof(__Z_SLIST));
	list->items=(void **)malloc(itemstotalsize);
	list->item_count=0;
	list->item_maxcount=item_maxcount;
	list->item_size=item_size;
	return (Z_SLIST)list;
}

void *download_thread(void *arg){
	__download_task* task=NULL;
	while(1){
		(__download_task *)task_slist_get(task_slist,0);
		if(task !=NULL){
			printf("New download task:\n");
			printf("Downloading... url=%s",task->url);
			sleep(1);
			printf("Finsh!  Path=%s",task->save_path);
			task_slist_delete(task_slist,0);	
		} 
		sleep(1);
	}
	return NULL;
} 

int init_service(void){
	task_slist = task_slist_new(sizeof(__download_task),MAX_TASK_CNT);
	pthread_create(&tid,NULL,download_thread,NULL);
	return tid; 
}

int deinit_service(void){
	int ret=-1;
	ret=pthread_cancel(tid);
	if(0==ret){
		pthread_join(tid,NULL);
	}
	return ret;
}

int main(){
	init_service();
	task_add("https://airec-init-data.oss-cn-beijing.aliyuncs.com/%E6%96%B0%E9%97%BB%E8%B5%84%E8%AE%AF/behavior_news.csv?spm=a2c4g.11186623.0.0.72bd6750Cq377h&file=behavior_news.csv","./test.csv");
	sleep(5);
	deinit_service();
	return 0;
}


