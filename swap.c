#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdbool.h>
#include<semaphore.h>
#include<glib.h>

//#define SWAP(X,Y)(int temp = X; X = Y; Y = temp;)

bool lock;
bool * waiting;
int p_Num;

void * thread(void *param){
    int t_num = *((int*)param);
    bool key;
   while(true){
     *(waiting+t_num) = true;
     key =true;
     while(*(waiting+t_num)!=false && key){SWAP(&key,&lock)}
     *(waiting+t_num) = false;
     printf("[%d] in C.S.\n",t_num);
    int j = (t_num+1)%p_Num;
    if(j == t_num)lock = false;
    else *(waiting+j) = false;
    printf("[%d] leave C.S.\n",t_num);
   }
}



int main(int argc,char *argv[]){

 if(argc!= 2){
    printf("Format ./<Program><pthread #>.\n");
    return -1;
  }
  int i;
  p_Num = atoi(argv[1]); 
  pthread_t t_Handles[atoi(argv[1])];
  waiting = (bool*)malloc(sizeof(bool)*atoi(argv[1]));
  lock = false;
  for(i = 0; i < atoi(argv[1]); i++){
    waiting[i] = false;
  } 

  for(i = 0; i < atoi(argv[1]); i++){
    int* Num = (int*)malloc(sizeof(int));
     *Num = i;
    pthread_create(&t_Handles[i],NULL,&thread,Num);
  }

  for(i = 0; i < atoi(argv[1]); i++){
    pthread_join(t_Handles[i],NULL);
  }
  exit(EXIT_SUCCESS);
}
