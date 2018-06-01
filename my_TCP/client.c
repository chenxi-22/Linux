#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
  if(argc != 3) {
    printf("usage %s [IP] [port]\n",argv[0]);
    return 1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    printf("socket error!\n");
    return 2;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(atoi(argv[2]));

  if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) { 
    printf("connect error!\n");
    close(sock);
    return 3;
  }

  char buf[1024];
  while(1) {
    buf[0] = 0;
    printf("please enter#");
    fflush(stdout);
    ssize_t s = read(0, buf, sizeof(buf) - 1);
    if(s > 0) {
      buf[s-1] = 0;
      if(strcmp("quit",buf) == 0) {
        break;
      }
      write(sock, buf, strlen(buf));
    }
  }
  close(sock);

  return 0;
}
