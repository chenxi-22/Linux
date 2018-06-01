#include "comm.h"


int commMsgQueue(int flag)
{
  key_t key = ftok(PATHNAME, PROJ_ID);
  if(key < 0)
  {
    printf("ftok error!\n");
    return -1;
  }
  
  int msgid = msgget(key, flag);
  if(msgid < 0)
  {
    printf("msgget error!\n");
    return -1;
  }

  return msgid;
}

int creatMsgQueue()
{
  return commMsgQueue(IPC_CREAT | IPC_EXCL | 0666);
}

int getMsgQueue()
{
  return commMsgQueue(IPC_CREAT);
}

int destroyMsgQueue(int msgid)
{
  if(msgctl(msgid, IPC_RMID, NULL) < 0)
  {
    printf("msgctl error!\n");
    return -1;
  }
  return 0;
}

int sendMsg(int msgid, int whotype, char* msg)
{
  struct msgbuf1 buf; 

  buf.mtype = whotype;
  strcpy(buf.mtext,msg);

  if(msgsnd(msgid, (void*)&buf, sizeof(buf.mtext), 0) < 0)
  {
    printf("msgsnd error!\n");
    return -1;
  }

  return 0;
}

int recvMsg(int msgid, int recvwhotype, char out[])
{
  struct msgbuf1 buf;

  if(msgrcv(msgid, (void*)&buf, sizeof(buf.mtext), recvwhotype, 0) < 0)
  {
    printf("msgrcv error!\n");
    return -1;
  }
  
  strcpy(out,buf.mtext);

  return 0;
}







