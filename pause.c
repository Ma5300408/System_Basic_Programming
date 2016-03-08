#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static void sigint_handler (int signo)
{

    printf("Caught SIGINT!\n");
    exit(EXIT_SUCCESS);
}

int main(void)
{
   if(signal (SIGINT,sigint_handler) == SIG_ERR){
       fprintf(stderr,"Cannot handler SIGINT!\n");
       exit(EXIT_FAILURE);
    }

   for(;;)pause();

   return 0;
}
