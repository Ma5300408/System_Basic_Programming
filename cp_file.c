#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<pthread.h>
#include<dirent.h>

#define READ_SIZE 1024



struct file_info {
  char * file_path;
  char * file_name;
};

struct file_info * path_info;
int shmid;
char *shmaddr, *s;

void * Copy(void * pArg){

     int tNum =  *((int*)pArg);
  char * BeWrittenFileName = (char*)malloc(sizeof(char)*(strlen(path_info[tNum].file_name)+6));
      char * sp; 
      int i;
      int index;
      index = 0;
      for(i =0 ; i < strlen(path_info[tNum].file_name); i++){
          if(path_info[tNum].file_name[i]=='.'){
                 index = i;
           }
      }
      if(index!=0){
  
         memcpy(BeWrittenFileName,path_info[tNum].file_name,sizeof(char)*index);                         sprintf(BeWrittenFileName+index,"%s%d%s","Copy",tNum,".txt");
         BeWrittenFileName[strlen(BeWrittenFileName)] = '\0';
        }
      else if(index == 0){
          memcpy(BeWrittenFileName,path_info[tNum].file_name,sizeof(char)*strlen(path_info[tNum].file_name));         sprintf(BeWrittenFileName+strlen(path_info[tNum].file_name),"%s%d","Copy",tNum);
         BeWrittenFileName[strlen(BeWrittenFileName)] = '\0';
      }
   
     // printf("%s\n",path_info[tNum].file_path);
     //DIR *pDir;
      
    //pDir = opendir(path_info[tNum].file_path);
     // if(pDir == NULL){
       //      printf("Cannot open Dir %s!\n",path_info[tNum].file_path);
        //   exit(-1);
      // }

     if(chdir(path_info[tNum].file_path) < 0){
         printf("fail!\n");
         exit(-1);
      }

     FILE * fp = fopen(BeWrittenFileName,"wb");
     size_t readn;
     size_t Fsize;       
     sp = shmaddr;
     Fsize = strlen(sp);
     // printf("%d\n",Fsize);
       
      while(Fsize > 0){
    //     printf("%d\n",Fsize);
         if(Fsize >= 1024){  
         fwrite(sp,sizeof(char),READ_SIZE,fp);
         fflush(fp);
         Fsize -= 1024;
         sp += READ_SIZE;
         }
         else{
           fwrite(sp,sizeof(char),Fsize,fp);
           fflush(fp);
           Fsize -=Fsize;
           sp += Fsize;
          } 
      }
     
    // closedir(pDir);
       chdir("./Linux_System_Programming/");
    
   pthread_exit(0);
}






int main(int argc,char * argv[]){

     if(argc < 3){
         printf("Format : ./<program> <../New_file_path/...> <../New_file_path/...>... <filename>\n");
         return -1;
     }

                 path_info = (struct file_info*)malloc(sizeof(struct file_info)*(argc-2));
 
    int r;
    for(r = 1; r < argc-1; r++){
          if(argv[r][0] != '.' && argv[r][1]!= '.' && argv[r][2]!= '/'){ printf("Format Error!\n"); return -1;}
          else{
                 path_info[r-1].file_path =(char*)malloc(sizeof(char)*(strlen(argv[r])+1));
                 memcpy(path_info[r-1].file_path,argv[r],sizeof(char)*strlen(argv[r]));
                 path_info[r-1].file_path[strlen(argv[r])] = '\0';
                
                 path_info[r-1].file_name = (char*)malloc(sizeof(char)*(strlen(argv[argc-1])+1));
                 path_info[r-1].file_name[strlen(argv[argc-1])] = '\0';

              }
    }
    char *file_name;
    file_name = (char*)malloc(sizeof(char)*(strlen(argv[argc-1])+1));
    int Num_Threads = argc-2; 
    
          

      memcpy(file_name,argv[argc-1],sizeof(char)*(strlen(argv[argc-1])));
      file_name[strlen(argv[argc-1])] = '\0';
     FILE * fp = fopen(file_name,"rb"); 

     if(!fp){
        printf("file open failed!\n");
        return -1;
      }
   

      fseek(fp,0,SEEK_END);
      int FSize = ftell(fp);
      rewind(fp);
 
      
     
      int shmid;

       shmid = shmget(IPC_PRIVATE,FSize,IPC_CREAT|0600);
       
       if(shmid < 0)
       {
          perror("get shm ipc_id error");
          return -1;
       }     

       shmaddr = (char*)shmat(shmid,NULL,0);
       if((int)shmaddr == -1)
       {
          perror("shmat addr error");
          return -1;
       }
       
       s = shmaddr;
        int readn;
        while( (readn = fread(s,sizeof(char),READ_SIZE,fp))!=0){
            s += readn;
        }
      
       //  printf("%s",shmaddr);
       pthread_t tHandles[Num_Threads];

        int i;
        for(i = 0; i < Num_Threads; i++){
            int *tnum = (int*)malloc(sizeof(int));
            *tnum = i;
          pthread_create(&tHandles[i],NULL,Copy,(void*)tnum);
        }

          int j;
          for(j = 0; j < Num_Threads; j++){
            pthread_join(tHandles[j],NULL);
         }

       shmdt(shmaddr);
       shmctl(shmid,IPC_RMID,NULL);


 return 0;
}
