#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define INT_SIZE 4

int main(int argc,char * argv[])
{

   if(argc!=2){
     printf("Correct Format: ./<program>  <num to fibonacci>.\n");
     return -1;
   }


   int num = atoi(argv[1]);
   int shmid;
   long long *shmaddr;
   struct shmid_ds buf;
   int flag = 0;
   int pid;
  
   shmid = shmget(IPC_PRIVATE,INT_SIZE*num,IPC_CREAT|0600);
  
   if(shmid < 0){
    printf("shmget failed! %s\n", strerror(errno));
    return -1;
   }

   int i = fork();



   if( i < 0){
      printf("fork failed! %s\n", strerror(errno));
       shmctl(shmid,IPC_RMID,NULL);
      return -1;
    }

   else if(i == 0){

      shmaddr = (long long*)shmat(shmid,NULL,0);
      if((int)shmaddr == -1)
       {
         perror("shmat addr error");
         return -1;
       }

     int j;
     int Fn_1;
     int Fn_2;
     long long * Fn;
     int temp;
     Fn = (long long*)malloc(sizeof(long long)*num);
     Fn_1 = 0;
     Fn_2 = 1;
     for(j = 0; j <= num ; j++){
         temp = Fn_2;

        Fn[j] = Fn_1 + Fn_2;
       
        Fn_1 = temp;
        Fn_2 = Fn[j];
       memcpy(shmaddr+j,Fn+j,sizeof(Fn[j]));  
     }
     //memcpy(shmaddr,&Fn,sizeof(Fn));
     shmdt(shmaddr);
    // printf("Answer: %d \n",Fn);
     exit(0);
    }

   else{
     wait(NULL);
      shmaddr = (long long*)shmat(shmid,NULL,0);
    
      if((int)shmaddr == -1)
       {
         perror("shmat addr error");
         return -1;
       }


      printf("Father wait son successfully.\n");
      int k;  
      for(k = 0; k <= num; k++)printf("Answer: %llu \n",shmaddr[k]);
      shmdt(shmaddr);
      shmctl(shmid,IPC_RMID,NULL);

   }

  return 0;
}
