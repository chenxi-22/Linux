#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
  if(argc != 3) {
    printf("usage %s [IP] [port]\n",argv[0]);
    return 1;
  }

  int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(listen_sock < 0) {
    printf("socket error!\n");
    return 2;
  }

  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port = htons(atoi(argv[2]));
  local.sin_addr.s_addr = inet_addr(argv[1]);

  if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0) {
    printf("bind error!\n");
    close(listen_sock);
    return 3;
  }

  if(listen(listen_sock, 5) < 0) {
    printf("listen error!\n");
    close(listen_sock);
    return 4;
  }

  char buf[1024];
  while(1) {
    buf[0] = 0;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    pid_t pid = fork();
    if(pid == 0) {
      if(fork() == 0) {
        int sock = accept(listen_sock, (struct sockaddr*)&client_addr, &len);
        if(sock < 0) {
          printf("accept error!\n");
          continue;
        }
        printf("[%s|%d] comming!\n",inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port));

        while(1){
          ssize_t s = read(sock, buf, sizeof(buf) - 1);
          if(s > 0) {
            buf[s] = 0;
            printf("[%s|%d]says#%s\n",inet_ntoa(client_addr.sin_addr), 
                   ntohs(client_addr.sin_port), buf);
            write(sock, buf, strlen(buf));
          } else {
            printf("[%s|%d] quit!\n",inet_ntoa(client_addr.sin_addr), 
                   ntohs(client_addr.sin_port));
            break;
          }
        }
        close(sock);
      } else {
        exit(1);
      }
    } else {
      waitpid(pid, NULL, 0);
    }
  }
  close(listen_sock);

  return 0;
}
