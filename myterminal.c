// Shell.

#include "types.h"
#include "user.h"
#include "fcntl.h"
// #include "string.h"

int present;

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

void separatecmd_array(char *buff, char **cmd_array, int *size)
{
  *size = 0;
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
  *size = idx;
}

char **make_tokens(char *cmd_)
{
  char *cmd_sp;
  char **my_array;
  my_array = (char **)malloc(50 * sizeof(my_array));
  int index = 0;
  int temp = 0;
  int mnd = 0;
  cmd_sp = (char *)malloc(200 * sizeof(cmd_sp));
  for (int i = 0; i < strlen(cmd_); i++)
  {
    if (cmd_[i] == '"' && mnd == 0)
      mnd = 1;

    if (cmd_[i] == '"' && mnd == 1)
      mnd = 0;
    if (mnd == 0)
    {
      if (cmd_[i] == ' ' || cmd_[i] == '\n')
      {

        cmd_sp[index] = '\0';

        if (index != 0)
        {
          my_array[temp] = (char *)malloc(200 * sizeof(cmd_sp));
          strcpy(my_array[temp], cmd_sp);
          temp++;
          index = 0;
        }
      }

      else
      {
        cmd_sp[index++] = cmd_[i];
      }
    }
    else
    {
      cmd_sp[index++] = cmd_[i];
    }
  }
  free(cmd_sp);
  my_array[temp] = 0;
  return my_array;
}

int isPresent(int operator, char ** cmd_array, int *lastIdx, int size, int *pindex, int *input, int *output, int *present)
{
  if (operator== 1)
  {
    for (int i = 0; i < size; i++)
    {
      if (strcmp(cmd_array[i], "&&") == 0)
      {
        *lastIdx = i;
        *present = 1;
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
        *present = 1;
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
          *present = 1;
          *pindex = j;
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
          *present = 1;
          *pindex = j;
          return 1;
        }
        j++;
      }
    }
  }
  else if (operator== 7)
  {
    int lessflag = 0;
    int greatflag = 0;
    for (int i = 0; i < size; i++)
    {
      int j = 0;
      while (cmd_array[i][j])
      {
        if (cmd_array[i][j] == '<')
        {
          *lastIdx = i;
          *present = 1;
          *pindex = j;
          *input = i;
          lessflag = 1;
        }
        j++;
      }
    }

    for (int i = 0; i < size; i++)
    {
      int j = 0;
      while (cmd_array[i][j])
      {
        if (cmd_array[i][j] == '>')
        {
          *lastIdx = i;
          *present = 1;
          *output = i;
          *pindex = j;
          greatflag = 1;
        }
        j++;
      }
    }
    if (lessflag && greatflag)
      return 1;
  }
  else if (operator== 5)
  {
    for (int i = 0; i < size; i++)
    {
      int j = 0;
      while (cmd_array[i][j])
      {
        if (cmd_array[i][j] == '<')
        {
          *lastIdx = i;
          *present = 1;
          *pindex = j;
          *input = i;
          // lessflag = 1;
          return 1;
        }
        j++;
      }
    }
  }
  else if (operator== 6)
  {
    for (int i = 0; i < size; i++)
    {
      int j = 0;
      while (cmd_array[i][j])
      {
        if (cmd_array[i][j] == '>')
        {
          *lastIdx = i;
          *present = 1;
          *output = i;
          *pindex = j;
          // greatflag = 1;
          return 1;
        }
        j++;
      }
    }
  }
  return 0;
}

int contains(char *buff, char **cmd_array, int *lastIdx, int *size, int *pindex, int *input, int *output, int *present)
{
  *lastIdx = 0;
  int operator= - 1;
  if (isPresent(1, cmd_array, lastIdx, *size, pindex, input, output, present) == 1)
  { //AND
    // printf(1, "Found &&\n");

    return 1;
  }
  else if (isPresent(2, cmd_array, lastIdx, *size, pindex, input, output, present) == 1)
  { //OR
    // printf(1, "Found ||\n");

    return 2;
  }
  else if (isPresent(3, cmd_array, lastIdx, *size, pindex, input, output, present) == 1)
  { //SEMI-COLON
    // printf(1, "Found ;\n");

    return 3;
  }
  else if (isPresent(4, cmd_array, lastIdx, *size, pindex, input, output, present) == 1)
  {
    // printf(1, "Found |\n");

    return 4;
  }
  else if (isPresent(7, cmd_array, lastIdx, *size, pindex, input, output, present) == 1)
  {
    //NORMAL
    return 7;
  }
  else if (isPresent(6, cmd_array, lastIdx, *size, pindex, input, output, present) == 1)
  {
    //NORMAL
    return 6;
  }
  else if (isPresent(5, cmd_array, lastIdx, *size, pindex, input, output, present) == 1)
  {
    //NORMAL
    return 5;
  }
  return operator;
}

void stringjoin(char **temp, char *res)
{
  int idx = 0;
  int i = 0;
  while (temp[i])
  {
    int j = 0;
    while (temp[i][j])
    {
      res[idx++] = temp[i][j];
      j++;
    }
    i++;
    if (temp[i])
    {
      res[idx++] = ' ';
    }
  }
  printf(1, "Inside join \n%s\n", res);
}

void utilityFunction(char *buf)
{
  // printf(1, "Receiving -------%s %d\n", buf, strlen(buf));
  char *cmd_array[20];
  int lastIdx;
  int size;
  int pindex;
  int input = -1;
  int output = -1;
  char *temp[10];
  char *temp1[10];
  char *temp2[10];
  char *lc;
  char *rc;
  int splitindex = -1;
  char **ltokenized;
  char **rtokenized;
  char *inputfilename;
  char *outputfilename;
  int pipefd[2];
  int present = -1;
  int fd1 = 0;
  memset(cmd_array, 0, sizeof(cmd_array));
  char backup[200];
  memset(backup, 0, sizeof(backup));
  for (int i = 0; i < strlen(buf); i++)
  {
    backup[i] = buf[i];
  }
  for (int i = 0; i < strlen(buf); i++)
  {
    // printf(1, "%c at %d\n", buf[i], i);
    if (buf[i] == '&' && buf[i + 1] == '&')
    {
      // printf(1, "%c && %d\n", buf[i], i);
      splitindex = i;
      break;
    }
    if (buf[i] == '|' && buf[i + 1] == '|')
    {
      // printf(1, "%c || %d\n", buf[i], i);
      splitindex = i;
      break;
    }
    if (buf[i] == '|')
    {
      // printf(1, "%c | %d\n", buf[i], i);
      // printf(1, "TTTTTRUE %d\n", i);
      splitindex = i;
      break;
    }

    if (buf[i] == ';')
    {
      // printf(1, "%c ; %d\n", buf[i], i);
      splitindex = i;
      break;
    }
    // printf(1,"%d lastchar", buf[i + 1]);
  }
  separatecmd_array(buf, cmd_array, &size);
  // char temp[200];

  int operator= contains(buf, cmd_array, &lastIdx, &size, &pindex, &input, &output, &present);
  // if (present == -1) {
  //   exec(cmd_array[0], cmd_array);
  // }
  int cid1;
  int pid1;
  // int pid2;
  int cid2;

  // int flag1 = 0;
  // int flag2 = 0;
  int fileDesc[2];
  char *leftCmnd[10];
  memset(leftCmnd, 0, sizeof(leftCmnd));
  // memset(temp, 0, sizeof(temp));
  memset(temp, 0, sizeof(temp));
  memset(temp1, 0, sizeof(temp1));
  memset(temp2, 0, sizeof(temp2));
  char *rightCmnd[10];
  memset(rightCmnd, 0, sizeof(rightCmnd));
  if (operator== 1 || operator== 2 || operator== 5 || operator== - 1 || operator== 6)
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
  // printf(1, "%d\n\n\n", operator);
  // int i = 0;
  // while (leftCmnd[i])
  // {
  //   // printf(1, "%s %d\n", leftCmnd[i], strlen(leftCmnd[i]));
  //   printf(1, "%s %s\n", leftCmnd[i], buff);

  //   i++;
  // }
  // i = 0;
  // printf(1, "Separated by");
  // while (rightCmnd[i])
  // {
  //   printf(1, "%s %d\n", rightCmnd[i], strlen(rightCmnd[i]));
  //   i++;
  // }
  if (strcmp("executeCommands", cmd_array[0]) == 0)
  {
    fd1 = open(cmd_array[1], O_CREATE | O_RDWR);
    memset(temp, 0, sizeof(temp));
    char ch;
    char *ptr = (char *)malloc(200);
    memset(ptr, 0, sizeof(ptr));
    char *head = ptr;
    while (read(fd1, &ch, 1) == 1)
    {
      *head = ch;
      head++;
      if (ch == '\n')
      {
        cid1 = fork();
        if (cid1 == 0)
        {
          utilityFunction(ptr);
        }
        wait(0);
        memset(ptr, 0, sizeof(ptr));
        head = ptr;
      }
    }
    exit(0);
  }
  switch (operator)
  {
  default:
    // printf(1, "Running from default\n");
    if (cmd_array[0] == 0)
      exit(-1);
    if (operator== 5)
    { //<<<<<<
      
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
      if (leftCmnd[0] == 0)
      {
        printf(2, "Illegal Command or Argument\n");
        return;
      }
      // int iidx = 0;
      // int ii = 0;
      // while (leftCmnd[ii])
      // {
      //   int jj = 0;
      //   while (leftCmnd[ii][jj])
      //   {
      //     temp1[iidx++] = leftCmnd[ii][jj];
      //     jj++;
      //   }
      //   ii++;
      //   if (leftCmnd[ii])
      //   {
      //     temp1[iidx++] = ' ';
      //   }
      // }

      int op = isPresent(5, leftCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      { //<<<<<<
        
        for (int i = 0; i < input; i++)
        {
          temp1[i] = leftCmnd[i];
        }
        int ee = 0;
        for (int i = input + 1; i < size; i++)
        {
          temp2[ee++] = leftCmnd[i];
        }
        pid1 = fork();
        if (pid1 == 0)
        {
          close(0);
          fileDesc[0] = open(temp2[0], O_CREATE | O_RDWR);
          // printf(1, "%d fd\n", fileDesc[0]);
          dup(fileDesc[0]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      op = isPresent(6, leftCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      {
        
        for (int i = 0; i < output; i++)
        {
          temp1[i] = leftCmnd[i];
        }
        int ee = 0;
        for (int i = output + 1; i < size; i++)
        {
          temp2[ee++] = leftCmnd[i];
        }
        cid1 = fork();
        if (cid1 == 0)
        {
          // printf(1, "Fork");
          fileDesc[1] = open(temp2[0], O_CREATE | O_RDWR);
          close(1);
          // printf(1, "%d fd\n", fileDesc[1]);
          dup(fileDesc[1]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      else
      {
        exec(leftCmnd[0], leftCmnd);
        printf(2, "Illegal Command or Argument\n");
      }
      // printf(1, "Giving -------%s %d\n", temp1, strlen(temp1));
      // utilityFunction(temp1);
      // exec(leftCmnd[0], leftCmnd);
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
      int op = isPresent(5, rightCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      { //<<<<<<
        
        for (int i = 0; i < input; i++)
        {
          temp1[i] = rightCmnd[i];
        }
        int ee = 0;
        for (int i = input + 1; i < size; i++)
        {
          temp2[ee++] = rightCmnd[i];
        }
        pid1 = fork();
        if (pid1 == 0)
        {
          close(0);
          fileDesc[0] = open(temp2[0], O_CREATE | O_RDWR);
          // printf(1, "%d fd\n", fileDesc[0]);
          dup(fileDesc[0]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      op = isPresent(6, rightCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      {
        
        for (int i = 0; i < output; i++)
        {
          temp1[i] = rightCmnd[i];
        }
        int ee = 0;
        for (int i = output + 1; i < size; i++)
        {
          temp2[ee++] = rightCmnd[i];
        }
        cid1 = fork();
        if (cid1 == 0)
        {
          // printf(1, "Fork");
          fileDesc[1] = open(temp2[0], O_CREATE | O_RDWR);
          close(1);
          // printf(1, "%d fd\n", fileDesc[1]);
          dup(fileDesc[1]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      else
      {
        exec(rightCmnd[0], rightCmnd);
        printf(2, "Illegal Command or Argument\n");
      }
      // stringjoin(rightCmnd, temp2);
      // exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    break;
  case 2: //OR
    cid1 = fork();
    if (cid1 == 0)
    {
      int op = isPresent(5, leftCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      { //<<<<<<
        
        for (int i = 0; i < input; i++)
        {
          temp1[i] = leftCmnd[i];
        }
        int ee = 0;
        for (int i = input + 1; i < size; i++)
        {
          temp2[ee++] = leftCmnd[i];
        }
        pid1 = fork();
        if (pid1 == 0)
        {
          close(0);
          fileDesc[0] = open(temp2[0], O_CREATE | O_RDWR);
          // printf(1, "%d fd\n", fileDesc[0]);
          dup(fileDesc[0]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      op = isPresent(6, leftCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      {
        
        for (int i = 0; i < output; i++)
        {
          temp1[i] = leftCmnd[i];
        }
        int ee = 0;
        for (int i = output + 1; i < size; i++)
        {
          temp2[ee++] = leftCmnd[i];
        }
        cid1 = fork();
        if (cid1 == 0)
        {
          // printf(1, "Fork");
          fileDesc[1] = open(temp2[0], O_CREATE | O_RDWR);
          close(1);
          // printf(1, "%d fd\n", fileDesc[1]);
          dup(fileDesc[1]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      else
      {
        exec(leftCmnd[0], leftCmnd);
        printf(2, "Illegal Command or Argument\n");
      }
      // stringjoin(leftCmnd, temp1);
      // exec(leftCmnd[0], leftCmnd);
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
      int op = isPresent(5, rightCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      { //<<<<<<
        
        for (int i = 0; i < input; i++)
        {
          temp1[i] = rightCmnd[i];
        }
        int ee = 0;
        for (int i = input + 1; i < size; i++)
        {
          temp2[ee++] = rightCmnd[i];
        }
        pid1 = fork();
        if (pid1 == 0)
        {
          close(0);
          fileDesc[0] = open(temp2[0], O_CREATE | O_RDWR);
          // printf(1, "%d fd\n", fileDesc[0]);
          dup(fileDesc[0]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      op = isPresent(6, rightCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      {
        
        for (int i = 0; i < output; i++)
        {
          temp1[i] = rightCmnd[i];
        }
        int ee = 0;
        for (int i = output + 1; i < size; i++)
        {
          temp2[ee++] = rightCmnd[i];
        }
        cid1 = fork();
        if (cid1 == 0)
        {
          // printf(1, "Fork");
          fileDesc[1] = open(temp2[0], O_CREATE | O_RDWR);
          close(1);
          // printf(1, "%d fd\n", fileDesc[1]);
          dup(fileDesc[1]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      else
      {
        exec(rightCmnd[0], rightCmnd);
        printf(2, "Illegal Command or Argument\n");
      }
      // stringjoin(rightCmnd, temp2);
      // exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    break;
  case 3: //SEMI-COLON
    // printf(1, "Found ;\n");
    cid1 = fork();
    if (cid1 == 0)
    {
      int op = isPresent(5, leftCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      { //<<<<<<
        
        for (int i = 0; i < input; i++)
        {
          temp1[i] = leftCmnd[i];
        }
        int ee = 0;
        for (int i = input + 1; i < size; i++)
        {
          temp2[ee++] = leftCmnd[i];
        }
        pid1 = fork();
        if (pid1 == 0)
        {
          close(0);
          fileDesc[0] = open(temp2[0], O_CREATE | O_RDWR);
          // printf(1, "%d fd\n", fileDesc[0]);
          dup(fileDesc[0]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      op = isPresent(6, leftCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      {
        
        for (int i = 0; i < output; i++)
        {
          temp1[i] = leftCmnd[i];
        }
        int ee = 0;
        for (int i = output + 1; i < size; i++)
        {
          temp2[ee++] = leftCmnd[i];
        }
        cid1 = fork();
        if (cid1 == 0)
        {
          // printf(1, "Fork");
          fileDesc[1] = open(temp2[0], O_CREATE | O_RDWR);
          close(1);
          // printf(1, "%d fd\n", fileDesc[1]);
          dup(fileDesc[1]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      else
      {
        exec(leftCmnd[0], leftCmnd);
        printf(2, "Illegal Command or Argument\n");
      }
      // stringjoin(leftCmnd, temp1);
      // exec(leftCmnd[0], leftCmnd);
    }
    cid2 = fork();
    if (cid2 == 0)
    {
      int op = isPresent(5, rightCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      { //<<<<<<
        
        for (int i = 0; i < input; i++)
        {
          temp1[i] = rightCmnd[i];
        }
        int ee = 0;
        for (int i = input + 1; i < size; i++)
        {
          temp2[ee++] = rightCmnd[i];
        }
        pid1 = fork();
        if (pid1 == 0)
        {
          close(0);
          fileDesc[0] = open(temp2[0], O_CREATE | O_RDWR);
          // printf(1, "%d fd\n", fileDesc[0]);
          dup(fileDesc[0]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      op = isPresent(6, rightCmnd, &lastIdx, size, &pindex, &input, &output, &present);
      if (op == 1)
      {
        
        for (int i = 0; i < output; i++)
        {
          temp1[i] = rightCmnd[i];
        }
        int ee = 0;
        for (int i = output + 1; i < size; i++)
        {
          temp2[ee++] = rightCmnd[i];
        }
        cid1 = fork();
        if (cid1 == 0)
        {
          // printf(1, "Fork");
          fileDesc[1] = open(temp2[0], O_CREATE | O_RDWR);
          close(1);
          // printf(1, "%d fd\n", fileDesc[1]);
          dup(fileDesc[1]);
          exec(temp1[0], temp1);
        }
        wait(0);
        break;
      }
      else
      {
        exec(rightCmnd[0], rightCmnd);
        printf(2, "Illegal Command or Argument\n");
      }
      // stringjoin(rightCmnd, temp2);
      // exec(rightCmnd[0], rightCmnd);
    }
    wait(0);
    wait(0);
    break;
  case 7:
  case 4: //PIPE
    // printf(1, "Found | %d and split %d\n", operator, splitindex);
    lc = malloc(200 * sizeof(lc));
    rc = malloc(200 * sizeof(rc));
    int lcindex = 0;
    int rcindex = 0;
    for (int i = 0; i < splitindex; i++)
    {
      lc[lcindex++] = backup[i];
    }

    lc[lcindex++] = ' ';
    lc[lcindex] = '\0';
    for (int i = splitindex + 1; i < strlen(backup); i++)
    {
      rc[rcindex++] = backup[i];
    }
    rc[rcindex] = '\0';

    int liind = -1;
    int roind = -1;
    for (int i = 0; i < strlen(lc); i++)
    {
      if (lc[i] == '<')
        liind = i;
    }
    for (int i = 0; i < strlen(rc); i++)
    {
      if (rc[i] == '>')
        roind = i;
    }

    if (liind == -1 && roind == -1)
    {
      ltokenized = make_tokens(lc);
      rtokenized = make_tokens(rc);
      if (pipe(pipefd) == -1)
      {
        exit(-1);
      }
      if (fork() == 0)
      {
        close(1);
        dup(pipefd[1]);
        close(pipefd[0]);
        close(pipefd[1]);
        exec(ltokenized[0], ltokenized);
      }
      if (fork() == 0)
      {
        close(0);
        dup(pipefd[0]);
        close(pipefd[0]);
        close(pipefd[1]);
        exec(rtokenized[0], rtokenized);
      }
      close(pipefd[0]);
      close(pipefd[1]);
      wait(0);
      wait(0);
    }

    // IO redirection case
    else
    {
      inputfilename = malloc(200 * sizeof(inputfilename));
      outputfilename = malloc(200 * sizeof(inputfilename));
      if (pipe(pipefd) == -1)
      {
        exit(-1);
      }
      int pd[2];
      if (liind != -1) // IO REDIRECTION IN LC
      {
        char *lci = malloc(200 * sizeof(lc));
        int lciindex = 0;
        for (int i = 0; i < liind; i++)
        {
          lci[lciindex++] = lc[i];
        }
        lci[lciindex] = '\0';
        ltokenized = make_tokens(lci);

        int filenameindex = 0;
        int i = liind + 1;
        while (lc[i] == ' ')
        {
          i++;
        }
        while (lc[i] != ' ')
        {
          inputfilename[filenameindex++] = lc[i];
          i++;
        }
        inputfilename[filenameindex] = '\0';

        if (roind != -1) // output redirect in rc as well lc
        {
          int rcoindex = 0;
          char *rci = malloc(200 * sizeof(lc));
          for (int i = 0; i < roind; i++)
          {
            rci[rcoindex++] = rc[i];
          }
          rci[rcoindex] = '\0';
          rtokenized = make_tokens(rci);
          int filenameindex = 0;
          int i = roind + 1;
          while (rc[i] == ' ')
          {
            i++;
          }
          while (rc[i] != ' ' && rc[i] != '\n')
          {
            outputfilename[filenameindex++] = rc[i];
            i++;
          }
          outputfilename[filenameindex] = '\0';
          pd[0] = open(inputfilename, O_CREATE | O_RDWR);
          pd[1] = open(outputfilename, O_CREATE | O_RDWR);
          if (fork() == 0)
          {
            close(0);
            dup(pd[0]);
            close(1);
            dup(pipefd[1]);
            close(pipefd[0]);
            close(pipefd[1]);
            exec(ltokenized[0], ltokenized);
          }
          if (fork() == 0)
          {
            close(1);
            dup(pd[1]);
            close(0);
            dup(pipefd[0]);
            close(pipefd[0]);
            close(pipefd[1]);
            exec(rtokenized[0], rtokenized);
          }
          close(pipefd[0]);
          close(pipefd[1]);
          wait(0);
          wait(0);
        }
        else // io not in rc only in lc
        {
          ltokenized = make_tokens(lci);
          rtokenized = make_tokens(rc);
          pd[0] = open(inputfilename, O_CREATE | O_RDWR);
          if (fork() == 0)
          {
            close(0);
            dup(pd[0]);
            close(1);
            dup(pipefd[1]);
            close(pipefd[0]);
            close(pipefd[1]);
            exec(ltokenized[0], ltokenized);
          }
          if (fork() == 0)
          {
            close(0);
            dup(pipefd[0]);
            close(pipefd[0]);
            close(pipefd[1]);
            exec(rtokenized[0], rtokenized);
          }
          close(pipefd[0]);
          close(pipefd[1]);
          wait(0);
          wait(0);
        }
      }
      else // io only in rc
      {
        ltokenized = make_tokens(lc);
        int rcoindex = 0;
        char *rci = malloc(200 * sizeof(lc));

        for (int i = 0; i < roind; i++)
        {
          rci[rcoindex++] = rc[i];
        }
        rci[rcoindex] = '\0';
        rtokenized = make_tokens(rci);
        int filenameindex = 0;
        int i = roind + 1;
        while (rc[i] == ' ')
        {
          i++;
        }
        while (rc[i] != ' ' && rc[i] != '\n')
        {
          outputfilename[filenameindex++] = rc[i];
          i++;
        }
        outputfilename[filenameindex] = '\0';
        rtokenized = make_tokens(rci);
        pd[1] = open(outputfilename, O_CREATE | O_RDWR);
        if (fork() == 0)
        {
          close(1);
          dup(pipefd[1]);
          close(pipefd[0]);
          close(pipefd[1]);
          exec(ltokenized[0], ltokenized);
        }
        if (fork() == 0)
        {
          close(0);
          dup(pipefd[0]);
          close(1);
          dup(pd[1]);
          close(pipefd[0]);
          close(pipefd[1]);
          exec(rtokenized[0], rtokenized);
        }
        close(pipefd[0]);
        close(pipefd[1]);
        wait(0);
        wait(0);
      }
    }
    break;
  case -1: //NORMAL EXEC
    if (cmd_array[0] == 0)
      exit(-1);
    exec(cmd_array[0], cmd_array);
    printf(2, "Illegal Command or Argument\n");
    break;
  }
  free(rightCmnd);
  free(leftCmnd);
  free(cmd_array);

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
      free(buf);
      exit(0);
    }
    //int i = 1;
    if (fork() == 0)
    {
      // printf(1, "Running util : %d\n", i);
      utilityFunction(buf);
      exit(0);
      // printf(1, "Completed util : %d\n", i);
      // i++;
    }
    wait(0);
  }

  exit(0);
}
