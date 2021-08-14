// Shell.

#include "types.h"
#include "user.h"
#include "fcntl.h"
// #include "string.h"
char *commands[20];
int leftIdx;
int size;
struct cmnds
{
  char *cmndstr;
  char *left;
  int hasOp;
  char *right;
  int cmndlen;
  int subcmndlen;
};

int stringcpy(char *s, const char *t, int n)
{

  while (n-- > 0 && (*s++ = *t++) != 0)
    ;
  while (n-- > 0)
    *s++ = 0;
  return 0;
}

int len(char *str)
{
  int n = 0;
  while (str[n])
    n++;
  return n;
}

void checkIfExit(char *command)
{
  char *end = "Exit";
  int kill = 1;
  char *check = command;
  for (int i = 0; i < 4; i++)
  {
    if (*check != end[i])
    {
      kill = 0;
      break;
    }
    check++;

    if (i == 3 && *check != '\0')
      kill = 0;
  }

  if (kill == 1)
    exit(0);
}

void separateCommands(char *buff)
{
  size = 0;
  char *ptr;
  int started = 0;
  int idx = 0;
  int i;
  for (ptr = buff, i = 0; buff[i]; i++, ptr++)
  {
    if (started == 0)
    {
      started = 1;
      commands[idx++] = ptr;
    }

    if (buff[i] == ' ')
    {
      started = 0;
      buff[i] = '\0';
    }
    if (!buff[i + 1])
    {
      *(ptr++) = '\0';
    }
  }
  size = idx;
}

int contains(char *buff)
{
  memset(commands, 0, sizeof(commands));
  separateCommands(buff);
  checkIfExit(commands[0]);
  int operator= - 1;
  leftIdx = 0;
  for (int i = 0; i < size; i++)
  {
    int index = 0;
    for (; commands[i][index];)
    {
      if (commands[i][index + 1])
      {
        if (commands[i][index] == '&' && commands[i][index + 1] == '&')
        {
          leftIdx = i;
          operator= 1;
          break;
        }
        if (commands[i][index] == '|' && commands[i][index + 1] == '|')
        {
          leftIdx = i;
          operator= 2;
          break;
        }
      }
      if (commands[i][index] == '|')
      {
        leftIdx = i;
        operator= 3;
        break;
      }
      if (commands[i][index] == ';')
      {
        leftIdx = i;
        operator= 4;
        break;
      }
      index++;
    }
  }
  return operator;
}

// int utilExec(char **token, int cmndno)
// {
//   if (cmndno == 0)
//     exit(-1);
//   char **token1;
//   char **token2;
//   switch (cmndno)
//   {
//   default:
//     printf(1, "Default case is executed \n");
//   case 4:
//     int fileDes[2];
//     if (pipe(fileDes) == -1)
//     {
//       printf(1, "Can not create pipe\n");
//       return -1;
//     }
//     int cid1;

//     if ((cid1 = fork()) < 0)
//     {

//       printf(1, "fork has failed\n");
//       exit(0);
//     }
//     else if (cid1 == 0)
//     {

//       close(fileDes[1]);
//       dup(fileDes[0]);
//       execute_cmnd(token, 6);
//       close(fileDes[0]);
//     }
//     else
//     {

//       close(fileDes[0]);
//       dup(fileDes[1]);
//       execute_cmnd(token, 7);
//       close(fileDes[1]);
//       wait(0);
//     }
//     break;
//   }
//   return 0;
// }

void utilityFunction(char *buf)
{
  int operator= contains(buf);
  int cid1;
  int cid2;
  int fileDesc[2];
  char *leftCmnd[10];
  memset(leftCmnd, 0, sizeof(leftCmnd));
  char *rightCmnd[10];
  memset(rightCmnd, 0, sizeof(rightCmnd));
  for (int i = 0; i < leftIdx; i++)
  {
    leftCmnd[i] = commands[i];
  }
  int idx = 0;
  for (int i = leftIdx + 1; i < size; i++)
  {
    rightCmnd[idx++] = commands[i];
  }
  int status;
  switch (operator)
  {
  default:
    printf(1, "Default\n");
    break;
  case 1: //AND
    cid1 = fork();
    if (cid1 == 0)
    {
      exec(leftCmnd[0], leftCmnd);
    }
    status = -1;
    while (cid1 != wait(&status))
      ;
    if (status != 0)
    {
      printf(1, "failed\n");
      return;
    }

    cid2 = fork();
    if (cid2 == 0)
    {
      exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    return;
  case 2: //OR
    cid1 = fork();
    if (cid1 == 0)
    {
      exec(leftCmnd[0], leftCmnd);
    }

    status = 0;
    wait(&status);
    if (status != -1) //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    {
      printf(1, "failed\n");
      return;
    }

    cid2 = fork();
    if (cid2 == 0)
    {
      exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    return;
  case 3: //PIPE
    if (pipe(fileDesc) < 0)
    {
      exit(-1);
    }
    cid1 = fork();
    if (cid1 == 0)
    {
      close(1);
      dup(fileDesc[1]);
      close(fileDesc[0]);
      close(fileDesc[1]);
      exec(leftCmnd[0], leftCmnd);
    }
    cid2 = fork();
    if (cid2 == 0)
    {
      close(0);
      dup(fileDesc[0]);
      close(fileDesc[0]);
      close(fileDesc[1]);
      exec(rightCmnd[0], rightCmnd);
    }
    close(fileDesc[0]);
    close(fileDesc[1]);
    wait(0);
    wait(0);
    return;
  case 4: //PARALLEL EXEC
    printf(1, "Default\n");
    break;
  case -1: //NORMAL EXEC
    if (commands[0] == 0)
      exit(-1);
    exec(commands[0], commands);
    printf(2, "exec %s failed\n", commands[0]);
    return;
  }
  return;
}

int main(void)
{
  static char buf[100];
  int fd;
  while ((fd = open("console", O_RDWR)) >= 0)
  {
    if (fd >= 3)
    {
      close(fd);
      break;
    }
  }

  while (1)
  {
    printf(2, "MyShell>");
    memset(buf, 0, sizeof(buf));
    gets(buf, sizeof(buf));
    if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
    {
      buf[strlen(buf) - 1] = 0; // chop \n
      if (chdir(buf + 3) < 0)
        printf(2, "cannot cd %s\n", buf + 3);
      continue;
    }

    if (fork() == 0)
    {
      utilityFunction(buf);
    }
    wait(0);
  }

  exit(0);
}
