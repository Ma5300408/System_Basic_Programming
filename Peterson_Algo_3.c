#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<semaphore.h>
#include<pthread.h>

bool flag[2];
int turn;
sem_t Assign_Turn;

void * TwoThread(void *param){

  int t_num = *((int*)param);

  while(true){

     flag[t_num] = true;
     sem_wait(&Assign_Turn);
     turn = (t_num+1 >=2) ? 0 : 1; 
     sem_post(&Assign_Turn);
     while(flag[turn] && turn != t_num){};
     printf("[%d] in C.S\n",t_num);
     flag[t_num] = false;
     printf("[%d] leave C.S\n",t_num);
   }

}


int main(int argc,char *argv[])
{

   pthread_t t_handles[2];
   sem_init(&Assign_Turn,0,1);
   int i;

   for(i = 0; i < 2; i++){ 
       int* Num = (int*)malloc(sizeof(int));
       *Num = i;
      pthread_create(&t_handles[i],0,&TwoThread,Num);
   }
   

  for(i = 0; i < 2; i++){
    pthread_join(t_handles[i],NULL);
   }


  exit(0);

}
