#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>

sem_t Customer,Barber,mutex;

int waiting = 0;
int n;
int Cn;
void cut_hair(void){
   printf("Barber is cutting someone's hair.\n");
}

void being_cutting(int Num){
   printf("Num[%d] is cutten hair by Barber.\n",Num);
}

void * barber(void * param){

   while(true){
      sem_wait(&Customer);
      sem_wait(&mutex);
       waiting = waiting -1;
      sem_post(&Barber);
      sem_post(&mutex);

      cut_hair();

    }
}


void * customer(void * param){

   int Num = *((int*)param);

    sem_wait(&mutex);
      if(waiting < n){
       sem_post(&Customer);
       sem_post(&mutex);
       sem_wait(&Barber);
       
        being_cutting(Num);
      }

   else sem_post(&mutex);

  pthread_exit(0);
}




int main(int argc,char * argv[])
{
   if(argc!=3){
     printf("Format ./<Program> <Waiting Line Size> <Customer #>.\n");
     return -1;
   }

    n = atoi(argv[1]);
    Cn = atoi(argv[2]);
    sem_init(&Customer,0,0);
    sem_init(&Barber,0,0);
    sem_init(&mutex,0,1);
    pthread_t Barber_Handle;
    pthread_t Customer_Handles[Cn];
  int i;
  for(i = 0; i < 1; i++){
    pthread_create(&Barber_Handle,0,&barber,NULL);
  }

  for(i = 0; i < Cn; i++){
     int * tNum = (int*)malloc(sizeof(int));
     *tNum = i;
    pthread_create(&Customer_Handles[i],0,&customer,tNum);
  }

  for(i = 0; i < Cn; i++){
     pthread_join(Customer_Handles[i],NULL);
  }


 exit(0);
}
