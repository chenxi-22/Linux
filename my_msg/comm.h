#pragma  once

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define PATHNAME "."
#define PROJ_ID 0x6666

#define SERVERTYPE 1
#define CLIENTTYPE 2

struct msgbuf1{
  long mtype;
  char mtext[1024];
};

int creatMsgQueue();
int getMsgQueue();
int destroyMsgQueue(int msgqid);
int sendMsg(int msgid, int whotype, char* msg);
int recvMsg(int msgid, int recvwhotype, char out[]);







