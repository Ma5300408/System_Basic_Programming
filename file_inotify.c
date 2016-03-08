#include<stdio.h>
#include<sys/inotify.h>
#include<errno.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 1024

int main(int argc,char * argv[])
{

  int fd;
   
  fd = inotify_init1 (0);
  if(fd == -1){
    perror("inotify_init1");
    exit(EXIT_FAILURE);
   }

   int wd;

   wd = inotify_add_watch(fd,"/home",IN_ALL_EVENTS);
    if(wd == -1){
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_LEN] __attribute__((aligned(4)));
    ssize_t len, i = 0;

   // len = read(fd,buf,BUF_LEN);
   
    while(1){
          
         memset(buf,0,sizeof(buf));

      len = read(fd,buf,BUF_LEN);       

      struct inotify_event *event = (struct inotify_event *)&buf[i];

       if(event->mask & IN_ACCESS)printf("IN_ACCESS!\n");
     
       if(event->mask & IN_MODIFY)printf("IN_MODIFY!\n");

       if(event->mask & IN_ATTRIB)printf("IN_ATTRIB!\n");

       if(event->mask & IN_CLOSE_WRITE)printf("IN_CLOSE_WRITE!\n");

       if(event->mask & IN_CLOSE_NOWRITE)printf("IN_CLOSE_NOWRITE!\n");

       if(event->mask & IN_OPEN)printf("IN_OPEN!\n");

       if(event->mask & IN_MOVED_FROM)printf("IN_MOVED_FROM!\n");

       if(event->mask & IN_MOVED_TO)printf("IN_MOVED_TO!\n");

       if(event->mask & IN_CREATE)printf("IN_CREATE!\n");

       if(event->mask & IN_DELETE)printf("IN_DELETE!\n");

        if(event->mask & IN_DELETE_SELF)printf("IN_DELETE_SELF!\n");

       if(event->mask & IN_MOVE_SELF)printf("IN_MOVE_SELF!\n");

      printf("wd=%d  cookie=%d len=%d dir=%s\n",
              event->wd, event->cookie, event->len,
               (event->mask & IN_ISDIR)? "yes":"no");
         


        if(event->len)printf("name=%s\n",event->name);
   
        i += sizeof(struct inotify_event) + event->len;
    }


  return 0;
}
