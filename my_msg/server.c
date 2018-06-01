#include "comm.h"

int main()
{
  int msgid = creatMsgQueue();
  char buf[1024];

  while(1)
  {
    printf("please wait client...\n"); 
    recvMsg(msgid, CLIENTTYPE, buf);
    printf("client says# %s",buf);
    
    printf("server says#");
    ssize_t s = read(0, buf, sizeof(buf)-1);
    if(s < 0)
    {
      printf("read error!\n");
      return -1;
    }
    else if(s > 0)
    {
      sendMsg(msgid, SERVERTYPE, buf);
      printf("send done,wait recv...\n");
    }
  }
  destroyMsgQueue(msgid);

  return 0;
}



