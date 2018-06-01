#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void printsigset(sigset_t* p)
{
  int i = 1;
  //调用sigismember函数目的是确定每个信号是否在此信号集当中去
  //如果在这个函数返回值为1，不在返回0
  for(; i < 32; ++i) {
    if(sigismember(p,i)) {
      printf("1");
    }else {
      printf("0");
    }
  }
  printf("\n");
}

int main()
{
  sigset_t s,p;//定义信号集s,p，将来s信号集为操作信号集，
               //p信号集为输出型信号集，作为输出型参数输入至sigpending函数内部

  //对信号集初始化，接着再对2号信号进行发送，使其进行未决，接着对s信号集进行阻塞设置
  //阻塞二号信号
  sigemptyset(&s);
  sigaddset(&s,2);
  sigprocmask(SIG_BLOCK, &s, NULL);

  //利用信号集p作为输出型参数,获取当前进程的未决信号集，将其放入至p信号集
  while(1) {//不断的获取打印，直到我们发送二号信号，由于二号信号被阻塞，可以看到未决信号集的变化
    sigpending(&p);
    printsigset(&p);
    sleep(1);
  }

  return 0;
}


