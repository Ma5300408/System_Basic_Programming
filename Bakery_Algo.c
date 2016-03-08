#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#define Num 100

pthread_cond_t notbusy;
pthread_mutex_t mutex;

int Max(int * Array){

  int max = -1e9;
  int i;
  for(i = 0; i < Num; i++){
      if( Array[i] > max )max = Array[i];
  }

  return max;
}

bool choose[Num];
int Number[Num];
int ID[Num];


int Assign_ID()
{
   int j;
   for(j = 0; j < Num; j++){
     if(ID[j]== 1e9)break;
   }
   if(j == 99)return 1;

   return 0;
}


void *Customer(void * param){

   int tNum = *((int*)param);


        ID[tNum] = getpid();
        if(Assign_ID())pthread_cond_wait(&notbusy,&mutex);
        else pthread_cond_broadcast(&notbusy);

  while(true){

     choose[tNum] = true;
     Number[tNum] = Max(Number)+1;
     choose[tNum] = false;

     int j;
     for(j = 0; j < Num; j++){
       while(choose[j]);
       while(Number[j] > 0 && Number[j] < Number[tNum] 
                           || (Number[j]==Number[tNum] && ID[j] < ID[tNum]));

      }
 
      printf(" [%d] Get in Bakery buy bread.\n",tNum);

      Number[tNum] = 0;
   
      printf("[%d] leave the Bakery.\n",tNum);

  }

}




int main()
{
   int i;
   for(i = 0; i < Num; i++)choose[i] = false;
   for(i = 0; i < Num; i++)Number[i] = 0;
   for(i = 0; i < Num; i++)ID[i] = 1e9; 
   pthread_mutex_init(&mutex,NULL);
   
   pthread_t Customer_tHandles[Num];

   for(i = 0; i < Num; i++){
      int *NUM = (int*)malloc(sizeof(int));
      *NUM = i;
     pthread_create(&Customer_tHandles[i],NULL,Customer,NUM);
   }


  sleep(30);



  exit(0);
}
