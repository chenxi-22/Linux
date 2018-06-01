#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
  if(argc != 3) { //判断命令行的合法性
    printf("usage %s [IP] [port]\n", argv[0]);
    return 1;
  }

  int sock = socket(AF_INET, SOCK_DGRAM, 0);//创建套接字
  if(sock < 0) {
    printf("socket error!\n");
    return 2;
  }

  //声明sockaddr_in结构体，并填充
  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port = htons(atoi(argv[2]));//转化端口号为网络字节格式
  //inet_addr返回的地址已经是网络字节格式
  //无需调用htonl转化为网络字节格式 
  local.sin_addr.s_addr = inet_addr(argv[1]);

  //绑定套接字
  if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0) {
    printf("bind error!\n");
    return 3;
  }

  char buf[1024];
  struct sockaddr_in client;
  while(1) {
    socklen_t len = sizeof(client);
    buf[0] = 0;
    ssize_t s = recvfrom(sock, buf, sizeof(buf) - 1, 0, 
                         (struct sockaddr*)&client, &len);
    if(s > 0) {
      buf[s] = 0;
      printf("[%s|%d]:%s\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port), buf);
      sendto(sock, buf, strlen(buf), 0, 
             (struct sockaddr*)&client, len);
    }
  }
  close(sock);

  return 0;
}
