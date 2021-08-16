// Shell.

#include "types.h"
#include "user.h"
#include "fcntl.h"
// #include "string.h"
char *cmd_array[20];
int size;
int pindex;
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

int checkIfExit(char *command)
{
  int isEqual = strcmp(command, "Exit\n");
  // printf(1, "Is equal : %d\n", isEqual);
  if (isEqual == 0)
    return isEqual;
  isEqual = strcmp(command, "Exit \n");
  return isEqual;
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

int isPresent(int operator, int * lastIdx)
{
  if (operator== 1)
  {
    for (int i = 0; i < size; i++)
    {
      if (strcmp(cmd_array[i], "&&") == 0)
      {
        *lastIdx = i;
        return 1;
      }
    }
  }
  else if (operator== 2)
  {
    for (int i = 0; i < size; i++)
    {
      if (strcmp(cmd_array[i], "||") == 0)
      {
        *lastIdx = i;
        return 1;
      }
    }
  }
  else if (operator== 4)
  {
    for (int i = 0; i < size; i++)
    {
      int j = 0;
      while (cmd_array[i][j])
      {
        if (cmd_array[i][j] == '|')
        {
          *lastIdx = i;
          pindex = j;
          return 1;
        }
        j++;
      }
    }
  }
  else if (operator== 3)
  {
    for (int i = 0; i < size; i++)
    {
      int j = 0;
      while (cmd_array[i][j])
      {
        if (cmd_array[i][j] == ';')
        {
          *lastIdx = i;
          pindex = j;
          return 1;
        }
        j++;
      }
    }
  }
  else if (operator== 5) //<<<<<<<
  {
    for (int i = 0; i < size; i++)
    {
      int j = 0;
      while (cmd_array[i][j])
      {
        if (cmd_array[i][j] == '<')
        {
          *lastIdx = i;
          pindex = j;
          return 1;
        }
        j++;
      }
    }
  }
  else if (operator== 6) //>>>>>>
  {
    for (int i = 0; i < size; i++)
    {
      int j = 0;
      while (cmd_array[i][j])
      {
        if (cmd_array[i][j] == '>')
        {
          *lastIdx = i;
          pindex = j;
          return 1;
        }
        j++;
      }
    }
  }
  return 0;
}

int contains(char *buff, int *lastIdx)
{
  memset(cmd_array, 0, sizeof(cmd_array));
  separatecmd_array(buff);
  int operator= - 1;
  if (isPresent(1, lastIdx) == 1)
  { //AND
    // printf(1, "Found &&\n");

    return 1;
  }
  else if (isPresent(2, lastIdx) == 1)
  { //OR
    // printf(1, "Found ||\n");

    return 2;
  }
  else if (isPresent(3, lastIdx) == 1)
  { //SEMI-COLON
    // printf(1, "Found ;\n");

    return 3;
  }
  else if (isPresent(4, lastIdx) == 1)
  { //PIPE
    // printf(1, "Found |\n");

    return 4;
  }
  else if (isPresent(5, lastIdx) == 1)
  { //
    return 5;
  }
  else if (isPresent(6, lastIdx) == 1)
  {
    return 6; //
  }
  return operator;
}

void utilityFunction(char *buf)
{
  int lastIdx = 0;
  int operator= contains(buf, &lastIdx);
  int cid1;
  int cid2;
  int fileDesc[2];
  char *leftCmnd[10];
  memset(leftCmnd, 0, sizeof(leftCmnd));
  char *rightCmnd[10];
  memset(rightCmnd, 0, sizeof(rightCmnd));
  if (operator== 1 || operator== 2 || operator== 5 || operator== 6)
  {
    for (int i = 0; i < lastIdx; i++)
    {
      leftCmnd[i] = cmd_array[i];
    }
    int idx = 0;
    for (int i = lastIdx + 1; i < size; i++)
    {
      rightCmnd[idx++] = cmd_array[i];
    }
  }
  else
  {
    for (int i = 0; i < lastIdx; i++)
    {
      leftCmnd[i] = cmd_array[i];
    }
    leftCmnd[lastIdx] = (char *)malloc(sizeof(char) * 200);
    int j = 0;
    for (j = 0; j < pindex; j++)
    {
      leftCmnd[lastIdx][j] = cmd_array[lastIdx][j];
    }
    leftCmnd[lastIdx][j] = '\0';
    if (cmd_array[lastIdx][pindex + 1] != '\0')
    {

      int jj = 0;
      rightCmnd[0] = (char *)malloc(sizeof(char) * 200);

      for (int j = pindex + 1; cmd_array[lastIdx][j]; j++)
      {
        rightCmnd[0][jj++] = cmd_array[lastIdx][j];
      }
      rightCmnd[0][jj] = '\0';
      int idx = 1;
      for (int i = lastIdx + 1; i < size; i++)
      {
        rightCmnd[idx++] = cmd_array[i];
      }
    }
    else
    {
      int idx = 0;
      for (int i = lastIdx + 1; i < size; i++)
      {
        rightCmnd[idx++] = cmd_array[i];
      }
    }
  }
  int status;
  switch (operator)
  {
  default: //NORMAL EXEC
    if (cmd_array[0] == 0)
      exit(-1);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (operator== 5)
    { //<<<<<<
      // printf(1, "Less\n");

      cid1 = fork();
      if (cid1 == 0)
      {
        close(0);
        fileDesc[0] = open(rightCmnd[0], O_CREATE | O_RDWR);
        // printf(1, "%d fd\n", fileDesc[0]);
        dup(fileDesc[0]);
        exec(leftCmnd[0], leftCmnd);
      }
      else
      {
        wait(0);
      }
    }
    else if (operator== 6)
    {
      // printf(1, "Greater\n");
      cid1 = fork();
      if (cid1 == 0)
      {
        // printf(1, "Fork");
        fileDesc[1] = open(rightCmnd[0], O_CREATE | O_RDWR);
        close(1);
        // printf(1, "%d fd\n", fileDesc[1]);
        dup(fileDesc[1]);
        exec(leftCmnd[0], leftCmnd);
      }
      else
      {
        wait(0);
      }
    }
    else
    {
      exec(leftCmnd[0], leftCmnd);
      printf(2, "Illegal Command or Argument\n");
    }

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
      // free(leftCmnd);
      // free(rightCmnd);
      break;
    }

    cid2 = fork();
    if (cid2 == 0)
    {
      exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    break;
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
      break;
    }

    cid2 = fork();
    if (cid2 == 0)
    {
      exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    break;
  case 3: //SEMI-COLON
    // printf(1, "Found ;\n");
    cid1 = fork();
    if (cid1 == 0)
    {
      exec(leftCmnd[0], leftCmnd);
    }
    cid2 = fork();
    if (cid2 == 0)
    {
      exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    wait(0);
    break;
  case 4: //PIPE
    // printf(1, "Found |\n");
    if (pipe(fileDesc) < 0)
    {
      // free(leftCmnd);
      // free(rightCmnd);
      break;
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
    break;
  }
  free(rightCmnd);
  free(leftCmnd);
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
  // int i = 1;
  while (1)
  {
    printf(2, "MyShell>");
    memset(buf, 0, sizeof(buf));
    gets(buf, sizeof(buf));
    if (checkIfExit(buf) == 0)
    {
      free(cmd_array);
      free(buf);
      exit(0);
    }
    //int i = 1;
    if (fork() == 0)
    {
      // printf(1, "Running util : %d\n", i);
      utilityFunction(buf);
      free(cmd_array);
      exit(0);
      // printf(1, "Completed util : %d\n", i);
      // i++;
    }
    wait(0);
  }

  exit(0);
}
