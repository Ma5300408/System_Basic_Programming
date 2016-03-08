#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <semaphore.h>
buffer_item buffer[BUFFER_SIZE];

int In = 0;
int Out = 0;
int n = 5;
int count = 0;
pthread_mutex_t countex;
sem_t empty,full,buffer_t;

//pthread_mutex_init (&countex,NULL);

int insert_item(buffer_item item){
     
    if( ((In+1)%BUFFER_SIZE) == Out || count == BUFFER_SIZE ){
         return -1;
    }
    else{
      buffer[In] = item;
      In = (In+1)%BUFFER_SIZE;
      
      pthread_mutex_lock(&countex);
       count++;
      pthread_mutex_unlock(&countex);

      return 0;
    }
}


int remove_item(buffer_item *item){

    Out = (Out+1)%BUFFER_SIZE;

   if(In == Out || count == 0){
     return -1;
    }

   else{
       
       *item = buffer[Out];
       buffer[Out] = 0;

      pthread_mutex_lock(&countex);
       count--;
      pthread_mutex_unlock(&countex);

       return 0;
    }
}



void *producer(void *param){
   buffer_item item;
  // srand(time(NULL));
  while(true){
   
   //sleep(rand()%n);
   item = rand()%10000;
   sem_wait(&empty);
   sem_wait(&buffer_t);
   if(insert_item(item))printf("report error condition");
   else printf("Producer produced %d\n",item);       
   sem_post(&buffer_t);
   sem_post(&full); 

 }


}

void *consumer(void *param){
     buffer_item item;

    while(true){
      sem_wait(&full);
      sem_wait(&buffer_t);
      //sleep(rand()%n);
      if(remove_item(&item))printf("report error condition");
      else printf("consumer consumed %d\n",item);
      sem_post(&buffer_t);
      sem_post(&empty);
   }
  

}


int main(int argc,char * argv[]){

  if(argc!=4){
    printf("Format ./<Program> <sleep sec> <Producer #> <Consumer #>");
    return -1;
   }

  pthread_mutex_init (&countex,NULL);
  sem_init(&empty,0,5);
  sem_init(&full,0,0);
  sem_init(&buffer_t,0,1);

  int i;
  int Sleep_Sec = atoi(argv[1]);
  int Producer_Num = atoi(argv[2]);
  int Consumer_Num = atoi(argv[3]);
  int err;  
   srand(time(NULL));
  pthread_t Producer_Handles[Producer_Num];
  pthread_t Consumer_Handles[Consumer_Num];

  for(i = 0; i < Producer_Num; i++){
   err = pthread_create(&Producer_Handles[i],NULL,producer,NULL);
    if(err!=0){perror("pthread_create_Producer");}
  }

  for(i = 0; i < Consumer_Num; i++){
   err = pthread_create(&Consumer_Handles[i],NULL,consumer,NULL);
    if(err!=0){perror("pthread_create_Consumer");}
  }
  

  sleep(Sleep_Sec);

  exit(0);
}
