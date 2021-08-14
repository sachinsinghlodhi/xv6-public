#include "types.h"
#include "user.h"
#include "fcntl.h"

char buff[100];
char *table[5] = {"&&","||","|",";"};
int endnow = 1;
void run(char *s)
{
    int type = -1;
    int pd[2];
    int left = -1;
    char *arr[10];
	memset(arr,0,sizeof(arr));
    char *ptr = s;
    int in = 0;
    int flag = 0;

    for(int i = 0; s[i]; i++,ptr++)
    {
        if(flag == 0)
        arr[in++] = ptr , flag = 1;

        if(s[i] == ' ')
        flag = 0 , s[i] = '\0';
        if(!s[i+1])
        *(ptr++) = '\0';
    }
    char *end = "Exit";
    endnow = 1;
    char *check = arr[0];
    for(int i = 0; i < 4; i++)
    {   
        if(*check != end[i])
        {
            endnow = 0;
            break;
        }
        check++;

        if(i == 3 && *check != '\0')
        endnow = 0;
    }

    if(endnow == 1)
    exit(0);
    if(left);
    for(int i = 0; i < in; i++)
    {
        int index = 0;   
        while(arr[i][index])
        {
            if(arr[i][index+1])
            {
                if(arr[i][index] == '&' && arr[i][index+1] == '&')
                {
                    type = 0;
                    left = i;
                    break;
                }
                if(arr[i][index] == '|' && arr[i][index+1] == '|')
                {
                    type = 1;
                    left = i;
                    break;
                }
            }
            if(arr[i][index] == '|')
            {
                type = 2;
                left = i;
                break;
            }          
            if(arr[i][index] == ';')
            {
                type = 3;
                left = i;
                break;
            } 
            index++;
        }
    }

    if(type == -1)
    {
        if(arr[0] == 0)
        exit(-1);
        // printf(1,"%s\n",arr[0]);
        exec(arr[0], arr);
        printf(2, "exec %s failed\n", arr[0]);
        return;
    }
    if(type == 0)
    {
        int pid1,pid2;
        char *argvl[10] , *argvr[10];
        memset(argvl,0,sizeof(argvl));
        memset(argvr,0,sizeof(argvr));

        for(int i = 0; i < left; i++)
        argvl[i] = arr[i];
        int idx = 0;
        for(int i = left+1; i < in; i++)
        argvr[idx++] = arr[i];

        pid1 = fork();
        if(pid1 == 0)
        {
            exec(argvl[0],argvl);
        }
        
        int status = -1;
        while(pid1 != wait(&status))
        ;
        if(status != 0)
        {
            printf(1,"failed\n");
            return;
        }

        pid2 = fork();
        if(pid2 == 0)
        {
            exec(argvr[0],argvr);
        }
        wait(0);
        return;
    }
    if(type == 1)
    {
        int pid1,pid2;
        char *argvl[10] , *argvr[10];
        memset(argvl,0,sizeof(argvl));
        memset(argvr,0,sizeof(argvr));

        for(int i = 0; i < left; i++)
        argvl[i] = arr[i];
        int idx = 0;
        for(int i = left+1; i < in; i++)
        argvr[idx++] = arr[i];

        pid1 = fork();
        if(pid1 == 0)
        {
            exec(argvl[0],argvl);
        }
        
        int status = 0;
        wait(&status);

        if(status != -1)
        {
            printf(1,"failed\n");
            return;
        }

        pid2 = fork();
        if(pid2 == 0)
        {
            exec(argvr[0],argvr);
        }
        wait(0);
        return;
    }
    if(type == 2)
    {
        char *argvl[10] , *argvr[10];
        memset(argvl,0,sizeof(argvl));
        memset(argvr,0,sizeof(argvr));

        for(int i = 0; i < left; i++)
        argvl[i] = arr[i];
        int idx = 0;
        for(int i = left+1; i < in; i++)
        argvr[idx++] = arr[i];

        if(pipe(pd) < 0)
        {
            exit(-1);
        }

        if(fork() == 0){
        close(1);
        dup(pd[1]);
        close(pd[0]);
        close(pd[1]);
        exec(argvl[0],argvl);
        }
        if(fork() == 0){
        close(0);
        dup(pd[0]);
        close(pd[0]);
        close(pd[1]);
        exec(argvr[0],argvr);
        }
        close(pd[0]);
        close(pd[1]);
        wait(0);
        wait(0);
        return;
    }
    if(type == 3)
    {
        int pid1,pid2;
        char *argvl[10] , *argvr[10];
        memset(argvl,0,sizeof(argvl));
        memset(argvr,0,sizeof(argvr));

        for(int i = 0; i < left; i++)
        argvl[i] = arr[i];
        int idx = 0;
        for(int i = left+1; i < in; i++)
        argvr[idx++] = arr[i];

        pid1 = fork();
        if(pid1 == 0)
        {
            exec(argvl[0],argvl);
        }

        pid2 = fork();
        if(pid2 == 0)
        {
            exec(argvr[0],argvr);
        }
        int count = 0;
        while(count != 2)
        {
            int temp = wait(0);
            if(temp == pid1 || temp == pid2)
            count++;
        }

        return;
    }
    
    
}

int main(void)
{
    int fd;

    while((fd = open("console", O_RDWR)) >= 0){
    if(fd >= 3){
      close(fd);
      break;
    }
  }

  while(1)
  {
      printf(2, "myshell>");
      for(int i = 0; i < sizeof(buff); i++)
      buff[i] = 0;
      gets(buff,sizeof(buff));
      if(fork() == 0)
      run(buff);
      wait(0);
  }
  exit(0);
}