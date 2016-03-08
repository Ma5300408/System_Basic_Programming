#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<semaphore.h>
#include<stdbool.h>
#include<time.h>
#include<unistd.h>

#define SIZE 1024

sem_t x,y,z,wsem,rsem;
int read_cnt = 0;
int write_cnt = 0;
//pthread_mutex_t mutex;
FILE * Wp;
struct file_id {
  FILE *Rp;
};

struct file_id * file_array;

void * writer(void *param){

 char c[2];   

    sem_wait(&y);

      write_cnt = write_cnt+1;
      if(write_cnt == 1)sem_wait(&rsem);

    sem_post(&y);

   sem_wait(&wsem);
   c[0] = rand()%94+32;
   c[1] = '\0';  
   fwrite(c,sizeof(char),1,Wp);
    fflush(Wp);
    printf("Write into:  %s\n\n",c);

   sem_wait(&y);

   write_cnt = write_cnt - 1;
   if(write_cnt == 0)sem_post(&rsem);

   sem_post(&wsem);
   sem_post(&y);
   
 
}

void * reader(void *param){
      int Num = *((int*)param);
     // file_id[Num].Rp = fopen("RW.txt","rb");
     char buff[SIZE];
   
        file_array[Num].Rp = fopen("RW.txt","rb");
        
       sem_wait(&z);
          sem_wait(&rsem);
            sem_wait(&x); 
  
             read_cnt = read_cnt + 1;
             if(read_cnt == 1)sem_wait(&wsem);
  
           sem_post(&x);
       
          //   fread(buff,sizeof(char),SIZE,file_array[Num].Rp);
          //   printf("[%d] Read from file :%s\n\n",Num,buff);
  
          sem_post(&rsem); 
        sem_post(&z);
          fread(buff,sizeof(char),SIZE,file_array[Num].Rp);
             printf("[%d] Read from file :%s\n\n",Num,buff);
        sem_wait(&x);
            read_cnt = read_cnt - 1;
           if(read_cnt == 0)sem_post(&wsem);
        sem_post(&x);  
 

       fclose(file_array[Num].Rp);
    

}



int main(int argc,char * argv[]){

   if(argc!=3){
     printf("Format: ./<program> <Writer #> <Reader #>.\n");
     return -1;
   }
   int i;
   int Writer_Num = atoi(argv[1]);
   int Reader_Num = atoi(argv[2]);
   pthread_t Writer_Handles[Writer_Num];
   pthread_t Reader_Handles[Reader_Num];
    //printf("hihi\n");
   file_array = (struct file_id*)malloc(sizeof(struct file_id)*Reader_Num);
    

//   printf("here!\n");

   sem_init(&x,0,1);
   sem_init(&y,0,1);
   sem_init(&z,0,1);
   sem_init(&rsem,0,1);
   sem_init(&wsem,0,1);
//   pthread_mutex_init(&mutex,0);
    Wp = fopen("RW.txt","wb");
 
   srand(time(NULL));
  
 for(i = 0; i < Writer_Num; i++){
      pthread_create(&Writer_Handles[i],NULL,writer,NULL);
    }
   
    for(i = 0; i < Reader_Num; i++){
        int * tNum = (int*)malloc(sizeof(int));
        *tNum = i;
      pthread_create(&Reader_Handles[i],NULL,reader,tNum);
    }

   sleep(10);
  fclose(Wp);

  exit(0);
}
