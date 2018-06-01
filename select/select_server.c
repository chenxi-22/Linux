#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

// 为了代码的可移植性，计算 fd_set 结构的大小
// 计算出来的结果是以字节为单位，乘以 8 就是可
// 容纳的比特位的总大小
#define MAXFD sizeof(fd_set) * 8 
#define INIT -1 // 初始化时将数组元素初始化为 INIT

int StarUp(int port) // 创建 listen_sock 函数
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = htonl(INADDR_ANY);
  local.sin_port = htons(port);

  if (bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0) {
    perror("bind");
    exit(2);
  }

  if (listen(sock, 5) < 0) {
    perror("listen");
    exit(3);
  }

  return sock;
}

void FdArrayInit(int* FdArray, int num) // 初始化只读文件描述符数组
{
  if (FdArray == NULL) {
    return;
  }

  int i = 0;
  for (; i < num; ++i) {
    FdArray[i] = INIT;
  }
  return;
}

int FdArrayAdd(int* FdArray, int num, int fd)
{
  if (FdArray == NULL) {
    return -1;
  }
  
  int i = 0;
  for (; i < num; ++i) {
    if (FdArray[i] == INIT) {
      FdArray[i] = fd;
      return 1;
    }
  }
  return 0; // 如果这里返回为 0 证明数组已经满了，服务器 busy 无法插入
}

void FdSet(int* FdArray, int num, fd_set* rfds)
{
  if (FdArray == NULL) {
    return;
  }

  int i = 0;
  for (; i < num; ++i) {
    if (FdArray[i] > INIT) {
      FD_SET(FdArray[i], rfds);
    }
  }
  return;
}

int GetMaxFd(int* FdArray, int num)
{
  if (FdArray == NULL) {
    return -1;
  }

  int i = 0;
  int max_fd = 0;
  for (; i < num; ++i) {
    if (FdArray[i] > max_fd) {
      max_fd = FdArray[i];
    }
  }
  return max_fd;
}

void FdArrayDelete(int* FdArray, int index, int num)
{
  if (FdArray == NULL || index > num) {
    return;
  }
  FdArray[index] = INIT;
}

void Service(int* FdArray, int num, fd_set* rfds)
{
  if (FdArray == NULL) {
    return;
  }

  int i = 0;
  for (; i < num; ++i) {
    if (FD_ISSET(FdArray[i], rfds)){
      if (i == 0) {
        // 监听描述符条件触发
        struct sockaddr_in client;
        socklen_t len = sizeof(client);

        // accept 连接
        int sock = accept(FdArray[0], (struct sockaddr*)&client, &len); 
        if (sock < 0) {
          printf("accept error!\n");
          continue;
        }

        printf("new client connect!\n");
        int ret = FdArrayAdd(FdArray, MAXFD, sock);
        if (ret == 0) { // 如果没有插入，证明数据满了，服务器繁忙
          printf("server busy!\n");
          continue;
        }
      } else {
        // 其他描述符条件触发
        char buf[1024] = {0};
        ssize_t s = read(FdArray[i], buf, sizeof(buf) - 1);
        if (s > 0) { // 读取数据成功
          printf("client says# %s\n",buf);
        } else if (s == 0) {
          close(FdArray[i]);
          printf("client quit!\n");
          FdArrayDelete(FdArray, i, num);
        } else {
          perror("read");
          close(FdArray[i]);
          FdArrayDelete(FdArray, i, num);
        }
      } // end else 
    }
  } // end for (; i < num; ++i)

  return;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("Usage %s [port]!\n",argv[0]);
    return 1;
  }

  int listen_sock = StarUp(atoi(argv[1])); // 封装创建listen_sock的函数

  // 定义只读文件描述符集
  fd_set rfds;
  
  // 定义一个数组保存将来的描述符，便于设置
  int FdArray[MAXFD];
  
  // 由于文件描述符都是大于等于 0 的，初始化数组时，初始化成 -1
  FdArrayInit(FdArray, MAXFD);
  FdArrayAdd(FdArray, MAXFD, listen_sock);

  while (1) {
    // 定义 timeval 结构，并初始化，设置等待时间为 5 秒
    struct timeval timeout = {7, 0}; 

    FD_ZERO(&rfds); // 清空只读文件描述符集
    FdSet(FdArray, MAXFD, &rfds); // 设置只读文件描述符集

    int max_fd = GetMaxFd(FdArray, MAXFD);
    if (max_fd < 0) {
      perror("GetMaxFd");
      return 2;
    }
    
    // select 传参第一个参数必须穿文件描述符最大值加一
    switch (select(max_fd + 1, &rfds, 
                   NULL, NULL, &timeout)) { 
    case 0:
      printf("select timeout...\n");
      break;
    case -1:
      perror("select");
      break;
    default:
      Service(FdArray, MAXFD, &rfds);
    }
  } // end while (1)

  return 0;
}
