#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
  if(argc != 3) {
    printf("usage %s [IP] [port]\n", argv[0]);
    return 1;
  }

  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) {
    printf("socket error!\n");
    return 2;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));
  server.sin_addr.s_addr = inet_addr(argv[1]);

  char buf[1024];
  struct sockaddr_in peer;
  while(1) {
    socklen_t len = sizeof(peer);
    buf[0] = 0;
    printf("please enter#");
    fflush(stdout);
    ssize_t s = read(0, buf, sizeof(buf) - 1);
    if(s < 0) {
      printf("read error!\n");
      return 3;
    }
    buf[s-1] = 0;
    sendto(sock, buf, strlen(buf), 0, 
           (struct sockaddr*)&server, sizeof(server));
    s = recvfrom(sock, buf, sizeof(buf) - 1, 0, 
             (struct sockaddr*)&peer, &len);
    if(s < 0) {
      printf("recvfrom error!\n");
      return 4;
    }
    buf[s] = 0;
    printf("peer return %s\n", buf);
  }
  close(sock);
  
  return 0;
}
