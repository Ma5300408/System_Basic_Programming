#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<semaphore.h>
#include<stdbool.h>

#define NUM 5

sem_t chopstick[NUM];

void * philosopher(void *param){
     int t_num = *((int*)param);
    while(true){
      printf("[%d] : hungry now.\n",t_num);
      if(t_num%2==0){
         sem_wait(&chopstick[t_num+1%NUM]);
         sem_wait(&chopstick[t_num]);
      }
      else{
        sem_wait(&chopstick[t_num]);
        sem_wait(&chopstick[t_num+1%NUM]);
      }
      printf("[%d]: eating now.\n",t_num);

   if(t_num%2== 0){
       sem_post(&chopstick[t_num+1%NUM]);
         sem_post(&chopstick[t_num]);
    } 
    else { 
      sem_post(&chopstick[t_num]);
      sem_post(&chopstick[t_num+1%NUM]);
     }
      printf("[%d]: thinking now.\n",t_num);
      sleep(1);
     }
}


int main(int argc,char * argv[]){

   pthread_t t_Handles[NUM];
   int i;
   for(i = 0; i < NUM; i++){
   sem_init(&chopstick[i],0,1);
   }
   for(i = 0; i < NUM; i++){
     int * num = (int*)malloc(sizeof(int));
     *num = i;
    pthread_create(&t_Handles[i],NULL,&philosopher,num);
   }
   for(i = 0; i < NUM; i++){
     pthread_join(t_Handles[i],NULL);
   }

   exit(EXIT_SUCCESS);
}
