#include<unistd.h>
#include<errno.h>
#include<stdio.h>


int main(int argc,char * argv[])
{
  if(argc!=2){
   printf("Format error! <pathname> to unlink ...\n");
  }

  char * pathname;
  int error;
 error = unlink(pathname);
  if(error!=0){
    if(errno)
   printf("%d\n",errno); 

  }

  return 0;
}
