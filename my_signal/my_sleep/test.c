#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_alarm(int signo)
{

}

int my_sleep(int t)
{
  struct sigaction o;
  struct sigaction n;

  int unslept = 0; 
  n.sa_handler = sig_alarm;
  sigemptyset(&n.sa_mask);
  n.sa_flags = 0;
  sigaction(SIGALRM, &n, &o);
  alarm(t);
  pause();
  unslept = alarm(0);
  sigaction(SIGALRM, &o, NULL);

  return unslept;
}

int main()
{
  while(1)
  {
    my_sleep(5);
    printf("5 seconds passed\n");
  }

  return 0;
}



