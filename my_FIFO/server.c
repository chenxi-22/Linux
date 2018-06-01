#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  if((mkfifo("FIFO",0644)) < 0)
  {
    printf("mkfifo error!\n");
    return -1;
  }

  int rfd = open("FIFO",O_RDONLY);
  if(rfd < 0)
  {
    printf("open error!\n");
    return -2;
  }

  char buf[1024];

  while(1)
  {
    printf("please wait client say.....\n");
    ssize_t s = read(rfd,buf,sizeof(buf)-1);

    if(s < 0)
    {
      printf("read error!\n");
      break;
    }
    else if(s > 0)
    {
      buf[s] = 0;
      printf("client says# %s\n",buf);
    }
    else
    {
      printf("client exit! now exit!\n");
      break;
    }
  }
  
  close(rfd);

  return 0;
}
