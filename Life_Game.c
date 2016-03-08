#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<stdbool.h>
#include<pthread.h>
#include<unistd.h>

#define NUM_THREADS 24
#define ROWS 16
#define COLS 24
#define N 4

struct Grid {
   bool live[ROWS][COLS];
};


struct Grid curr;
struct Grid next;


void * computeNextGen(void * pArg)
{
     int tNum = *((int*)pArg);  
     

     int Start_Row = tNum/(COLS/N); 
     int End_Row = tNum/(COLS/N) + 1;
     
     Start_Row = Start_Row * 4;
     End_Row = End_Row * 4 - 1;
 
     
      int Start_Col =  tNum%(COLS/N);
      int End_Col = tNum%(COLS/N) + 1;

      Start_Col = Start_Col * 4;
      End_Col = End_Col * 4 - 1;

      int count;
      int i , j;

      for(i = Start_Row; i <= End_Row; i++){
         for(j = Start_Col; j <= End_Col; j++){
          count = 0;

           if(i-1 >= 0 && j-1 >= 0){
               if(curr.live[i-1][j-1] == true ) count++; // NW neighbor
            }

           if(i-1>=0){
              if(curr.live[i-1][j] == true ) count++;  // N neighbor
           }

           if(i-1 >= 0 && j+1 < COLS){
              if(curr.live[i-1][j+1] == true )count++; // NE neighbor
           }

           if(j-1 >= 0){
                 if(curr.live[i][j-1] == true) count++; // W neighbor
           }

           if(j+1 < COLS){
               if(curr.live[i][j+1] == true) count++; // E neighbor
           }
          
          if(i+1 < ROWS && j-1 >=0){
               if(curr.live[i+1][j-1] == true )count++; // SW neighbor
           }

          if(i+1 < ROWS){
              if(curr.live[i+1][j] == true) count++; // S neighbor
          }

          if(i+1 < ROWS && j+1 < COLS){
               if(curr.live[i+1][j+1] == true) count++;  // SE neighbor
          }
           

          if(count <= 1 || count >= 4)next.live[i][j] = false;
          else if(curr.live[i][j] == true && (count == 2 || count == 3))
           next.live[i][j] = true;
          else if(curr.live[i][j] == false && count == 3)
           next.live[i][j] = true;
          else
           next.live[i][j] = false;

         }
      }
      
   return;
}




int main(){
   int i,j;
   for(i = 0 ; i < ROWS; i++)
     for(j = 0; j < COLS; j++){
       curr.live[i][i] = true;
       curr.live[i][i+1] = true;
       next.live[i][j] = false;
    }


    pthread_t t_Handle[NUM_THREADS];
 
   while(1){
    int k;
    for(k = 0; k < NUM_THREADS; k++){
      int *pNum = (int*)malloc(sizeof(int));
      *pNum = k;
      pthread_create(&t_Handle[k],NULL,computeNextGen,(void*)pNum);

    }

    for(k = 0; k < NUM_THREADS; k++){
      pthread_join(t_Handle[k],NULL);
    }
 
        
     for(i = 0 ; i < ROWS; i++){
     for(j = 0; j < COLS; j++){
      if(next.live[i][j]){
         printf("X");
      }
      else printf("-");
    }

    printf("\n");
  }

//   memcpy(&curr,&next,sizeof(struct Grid));
       curr = next;
  //   memset(&next,0,sizeof(struct Grid));
     sleep(1);
     printf("\n\n");
 }
    

  return 0;
}
