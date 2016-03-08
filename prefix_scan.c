#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>

#define NUM_THREADS 4


int inTotals[NUM_THREADS];
int outTotals[NUM_THREADS];
int N;
int *A;
sem_t  step1,step2,block;

void * prefixScan(void * pArg)
{

   int tNum = *((int*)pArg);
   int start, end, i;
   int lPrefixTotal;

   free(pArg);

   start = (N/NUM_THREADS) * tNum;
   end = (N/NUM_THREADS) * (tNum+1);
   if(tNum == (NUM_THREADS-1))end = N;

   
   for(i = start+1; i < end; i++)A[i] = A[i-1] + A[i];
   
   inTotals[tNum] = A[end-1];
   sem_post(&step1);
   printf("step1 sem_post!\n");

   sem_wait(&step2);
   printf("step2 wait!\n");
   ////wait Step2 completion//////


   //Step3
   lPrefixTotal = outTotals[tNum];
   for(i = start; i < end; i++)A[i] = lPrefixTotal + A[i];
   printf("finished step3.\n");
   pthread_exit(0);
}


int main(int argc,char * argv[])
{
    int i,j;
    pthread_t t_Handles[NUM_THREADS];
    N = 16;
    A = (int*)malloc(sizeof(int)*N);


    

     int err = sem_init(&step1,0,0);
        if(err !=0)printf("sem_init failed!\n");
        err = sem_init(&step2,0,0);
        if(err !=0)printf("sem_init failed!\n");
   




    for(i = 0; i < N; i++)scanf("%d",&A[i]); 
   
   
   

   for(i = 0; i< NUM_THREADS; i++){
      int *tnum = (int*)malloc(sizeof(int));
       *tnum = i;
    //   printf("%d\n",i);
       pthread_create(&t_Handles[i],NULL,prefixScan,(void*)tnum);
    }
       printf("create finished!\n");
     //for(j = 0; j < NUM_THREADS; j++) sem_wait(sem[j]);
//      sem_wait(&block);
      printf("step1 wait!\n");
      sem_wait(&step1); ////wait for Step1 finished.
      sem_wait(&step1);
      sem_wait(&step1);
      sem_wait(&step1);
   
     

   outTotals[0] = 0;
   for(j = 1; j < NUM_THREADS; j++)outTotals[j] = outTotals[j-1]+ inTotals[j-1];
      //sleep(3);
  // for(j = 0; j < NUM_THREADS; j++) sem_post(sem[j]); 
       
       sem_post(&step2); ///signal Step2/////
       printf("step2 post!\n");
       sem_post(&step2);
       sem_post(&step2);
       sem_post(&step2);


   for(j = 0; j < NUM_THREADS; j++){
     pthread_join(t_Handles[j],NULL);

   }
  

  for(i = 0; i < N; i++)printf(" %d ",A[i]);

  return 0;
}


