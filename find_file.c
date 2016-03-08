#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>


int find_file_in_dir(const char * path,const char *file)
{
    struct dirent *entry;
    int ret = 1;
    DIR *dir;

     dir = opendir(path);

     errno = 0;
     
      while((entry = readdir(dir)) != NULL){
            if(strcmp(entry->d_name,file) == 0){
               ret = 0;
               break;
            }


      }

     if(errno && !entry)
         perror("readdir!");

    closedir(dir);
    return ret;

}




int main(int argc,char * argv[])
{

    char *path;
    char *file;

    if(argc!=3){
     printf("Format error!\n");
     return -1;
    }

    path = argv[1];
    file = argv[2];


   printf("The result of find file in directory:%d",find_file_in_dir(path,file));
    sleep(1);

  return 0;
}
