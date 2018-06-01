#include "comm.h"

int main()
{
  int msgid = getMsgQueue();
  char buf[1024];

  while(1)
  {
    printf("please enter#");
    fflush(stdout);
    ssize_t s = read(0, buf, sizeof(buf)-1);
    if(s < 0)
    {
      printf("read error!\n");
      return -1;
    }
    else if(s > 0)
    {
      buf[s] = 0;
      sendMsg(msgid, CLIENTTYPE, buf);
      printf("send done,wait recv...\n");
    }
    recvMsg(msgid, SERVERTYPE, buf);
    printf("server says# %s",buf);
  }

  return 0;
}



