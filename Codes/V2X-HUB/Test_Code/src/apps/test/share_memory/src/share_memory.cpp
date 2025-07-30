#include <stdio.h>
#include <iostream>

#include <sys/ipc.h> 
#include <sys/shm.h> 

#define KEY_NUM 1234 
#define SHARE_MEMORY_HEADER_SIZE 24
#define MEM_SIZE 1024  


int main() {

	int shmid;
	void *address;
	if( (shmid = shmget(KEY_NUM, MEM_SIZE, IPC_CREAT | 0777)) != (-1) ) {
    
		address = shmat(shmid, NULL, 0);
		printf("공유메모리 만들기 성공 ! \n");
		printf("shmid = %d\n",shmid);
		printf("공유메모리 주소 : %p\n", address);
        
	} else printf("에러 입니다 !\n");
	
	shmat(shmid, shmaddr, 
	
	return 0;
    
}

