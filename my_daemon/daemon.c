#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

void mydeamon()
{
  int fd0;
  pid_t id;
  struct sigaction sa;

  //设置屏蔽字为0
  umask(0);

  id = fork();
  if(id < 0)
  {
    printf("fork error!\n");
    exit(1);
  }
  //让父进程退出
  if(id > 0)
  {
    exit(0);
  }

  if(setsid() < 0)
  {
    printf("setsid error!\n");
    exit(1);
  }
  //初始信号集
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = SIG_IGN;
  sa.sa_flags = 0;
  
  //忽略SIGCHLD信号
  if(sigaction(SIGCHLD, &sa, NULL) < 0)
  {
    perror("sigaction");
    exit(1);
  }

  fd0 = open("/dev/null", O_RDWR);
  close(0);
  dup2(fd0, 1);
  dup2(fd0, 2);
}

int main()
{
  mydeamon();

  while(1) {
    sleep(1);
  }

  return 0;
}



