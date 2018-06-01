#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
  int wfd = open("FIFO",O_WRONLY);
  if(wfd < 0)
  {
    printf("open error!\n");
    return -1;
  }

  while(1)
  {
    char buf[1024];
    printf("please enter->");
    fflush(stdout);
    ssize_t s = read(0,buf,sizeof(buf)-1);
    if(s <= 0)
    {
      printf("read error!\n");
      break;
    }
    else if(s > 0)
    {
      buf[s] = 0;
      write(wfd,buf,strlen(buf));
    }
  }

  close(wfd);

  return 0;
}
