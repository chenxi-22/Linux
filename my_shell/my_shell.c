#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void break_buf(char* buf,char** argv)
{
  if(buf == NULL)
  {
    return;
  }
  if(argv == NULL)
  {
    return;
  }

  char* ptr = strtok(buf," ");
  int argc = 0;
  
  argv[argc] = ptr;

  while(ptr != NULL)
  {
    ptr = strtok(NULL," ");
    argv[argc++] = ptr;
  }
  argv[argc] = NULL;
  return;
}

void my_shell(char** argv)
{
  if(argv == NULL)
  {
    return;
  }

  pid_t pid = fork();

  if(pid < 0)
  {
    printf("fork error!\n");
    return;
  }
  else if(pid > 0)
  {
    int status;
    wait(&status);
  }
  else
  {
    execvp(argv[0],argv);
  }
  return;
}

int main()
{
  while(1)
  {
    char buf[256];
    printf("my shell-> ");
    scanf("%s",buf);
    char* argv[10];

    break_buf(buf,argv);
    my_shell(argv);
  }
}







//
//
//
//#include <stdio.h>
//#include <sys/wait.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdlib.h>
//
//#include <ctype.h>
//
//char* argv[8];
//int argc = 0;
//
//void do_parse(char* buf)
//{
//    int i;
//    int status = 0;
//
//    for(argc=i=0; buf[i]; i++)
//    {
//        if(!isspace(buf[i]) && status == 0)
//        {
//            argv[argc++] = buf+i;
//            status =1;
//        }
//        else if(isspace(buf[i]))
//        {
//            status = 0;
//            buf[i] = 0;
//        }
//    }
//    argv[argc] = NULL;
//}
//
//void do_execute()
//{
//    pid_t pid = fork();
//
//    if(pid < 0)
//    {
//        perror("fork");
//        exit(1);
//    }
//    else if(pid == 0)
//    {
//        execvp(argv[0],argv);
//        perror("execvp");
//    }
//    else
//    {
//        int st;
//        while( wait(&st) != pid )
//        {
//            ;
//        }
//    }
//}
//
//int main()
//{
//    char buf[1024] = {};
//
//    while(1)
//    {
//        printf("myshell###");
//        scanf("%[^\n]%*c",buf);
//        do_parse(buf);
//        do_execute();
//    }
//
//    return 0;
//}
//


















