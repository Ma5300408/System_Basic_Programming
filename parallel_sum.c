#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define NUM_THREADS 4

int N; // number of elements in array X
int *X;
int gSum[NUM_THREADS];

void *Summation(void *pArg)
{
  int tNum = *((int*)pArg);
  int ISum = 0;
  int start , end;
  
  start = (N/NUM_THREADS) * tNum;
  end = (N/NUM_THREADS) * (tNum + 1);
  if(tNum == (NUM_THREADS-1))end = N;
  int i;
  for(i = start; i < end; i++)ISum += X[i];
  gSum[tNum] = ISum;
  free(pArg);
}


int main(int argc,char * argv[])
{
   int j, sum = 0;
   pthread_t t_Handles [NUM_THREADS];
   N = 8;
   X = (int*)malloc(sizeof(int)*N);
   for(j = 0; j < N; j++)scanf("%d",&X[j]);
   
   for(j = 0; j < NUM_THREADS; j++){
    int *threadNum = (int*)malloc(sizeof(int));
    *threadNum = j;
     pthread_create(&t_Handles[j],NULL,Summation,(void*)threadNum);

   }


    for(j = 0; j < NUM_THREADS; j++){
      pthread_join(t_Handles[j],NULL);
      sum += gSum[j];
    }

  printf("The sum of array element id %d\n",sum);
  return 0;
}
