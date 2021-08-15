// Shell.

#include "types.h"
#include "user.h"
#include "fcntl.h"
// #include "string.h"
char *cmd_array[20];
int lastIdx;
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

void separatecmd_array(char *buff)
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
      cmd_array[idx++] = ptr;
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

int isPresent(int operator)
{
  if (operator== 1)
  {
    for (int i = 0; i < size; i++)
    {
      int index = 0;
      for (; cmd_array[i][index];)
      {
        if (cmd_array[i][index + 1])
        {
          if (cmd_array[i][index] == '&' && cmd_array[i][index + 1] == '&')
          {
            lastIdx = i;
            return 1;
          }
        }
        index++;
      }
    }
  }
  else if (operator== 2)
  {
    for (int i = 0; i < size; i++)
    {
      int index = 0;
      for (; cmd_array[i][index];)
      {
        if (cmd_array[i][index + 1])
        {
          if (cmd_array[i][index] == '|' && cmd_array[i][index + 1] == '|')
          {
            lastIdx = i;
            return 1;
          }
        }
        index++;
      }
    }
  }
  else if (operator== 4)
  {
    for (int i = 0; i < size; i++)
    {
      int index = 0;
      for (; cmd_array[i][index];)
      {

        if (cmd_array[i][index] == '|')
        {
          lastIdx = i;
          return 1;
        }

        index++;
      }
    }
  }
  else if (operator== 3)
  {
    for (int i = 0; i < size; i++)
    {
      int index = 0;
      for (; cmd_array[i][index];)
      {

        if (cmd_array[i][index] == ';')
        {
          lastIdx = i;
          return 1;
        }
        index++;
      }
    }
  }
  return 0;
}

int contains(char *buff)
{
  memset(cmd_array, 0, sizeof(cmd_array));
  separatecmd_array(buff);
  checkIfExit(cmd_array[0]);
  int operator= - 1;
  lastIdx = 0;
  if (isPresent(1) == 1)
  { //AND
    // printf(1, "Found &&\n");

    return 1;
  }
  else if (isPresent(2) == 1)
  { //OR
    // printf(1, "Found ||\n");

    return 2;
  }
  else if (isPresent(3) == 1)
  { //SEMI-COLON
    // printf(1, "Found ;\n");

    return 3;
  }
  else if (isPresent(4) == 1)
  { //PIPE
    // printf(1, "Found |\n");

    return 4;
  } //NORMAL
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
  for (int i = 0; i < lastIdx; i++)
  {
    leftCmnd[i] = cmd_array[i];
  }
  int idx = 0;
  for (int i = lastIdx + 1; i < size; i++)
  {
    rightCmnd[idx++] = cmd_array[i];
  }
  int status;
  switch (operator)
  {
  default:
    printf(1, "Illegal Command or Argument\n");
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
    if (status == -1)
    {
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
    if (status == 0)
    {
      return;
    }

    cid2 = fork();
    if (cid2 == 0)
    {
      exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    return;
  case 3: //SEMI-COLON
    cid1 = fork();
    if (cid1== 0) {
      exec(leftCmnd[0], leftCmnd);
    }
    cid2 = fork();
    if (cid2 == 0) {
      exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    wait(0);
    return;
  case 4: //PIPE
    if (pipe(fileDesc) < 0)
    {
      return;
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
  case -1: //NORMAL EXEC
    if (cmd_array[0] == 0)
      exit(-1);
    exec(cmd_array[0], cmd_array);
    printf(2, "exec %s failed\n", cmd_array[0]);
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
