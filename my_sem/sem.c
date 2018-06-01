#include "comm.h"

int main()
{
  int _semid = createSemSet(1);
  initSem(_semid, 0,1);
  pid_t pid = fork();
  if(pid < 0)
  {
    printf("fork error!\n");
    return -1;
  }

  if(pid == 0)
  {
    int semid = getSemSet(0);
    while(1)
    {
      P(semid, 0);
      printf("A");
      fflush(stdout);
      usleep(123456);
      printf("A ");
      fflush(stdout);
      usleep(123456);
      V(semid, 0);
    }
  }
  else
  {
    while(1)
    {
      P(_semid, 0);
      printf("B");
      fflush(stdout);
      usleep(123456);
      printf("B ");
      fflush(stdout);
      usleep(123456);
      V(_semid, 0);
    }
    wait(NULL);
  }
  destroySemSet(_semid);

  return 0;
}
