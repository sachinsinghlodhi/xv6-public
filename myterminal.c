// Shell.

#include "types.h"
#include "user.h"
#include "fcntl.h"
// #include "string.h"

struct cmnds
{
  char *cmndstr;
  char *left;
  int hasOp;
  char *right;
  int type;
  int cmndlen;
  int subcmndlen;
};


int stringcpy(char *s, const char *t, int n)
{

  while (n-- > 0 && (*s++ = *t++) != 0);
  while (n-- > 0)
    *s++ = 0;
  return 0;
}

int len(char *str)
{
  int n = 0;
  while(str[n]) n++;
  return n;
}

struct cmnds *contains(struct cmnds *curr, char *operator)
{
  int i = 0, hasOp = 0, index = 0; 
  int command_len = curr->cmndlen, string_len = len(operator);
  char *command = curr->cmndstr, *ptr = operator;

  for (i = 0; i < command_len - string_len; i++)
  {
    hasOp = 0;
    for (int j = 0; j < string_len; j++)
    {
      if (ptr[j] != command[i + j])
      {

        hasOp = 1;
        break;
      }
    }
    if (hasOp == 0)
    {
      index = i;
      break;
    }
  }

  if (index == 0)
  {

    curr->hasOp = 0;
    return (struct cmnds *)curr;
  }
  curr->hasOp = 1;

  curr->left = malloc((index) * sizeof(char));
  memset(curr->left, 0, sizeof(curr->left));
  stringcpy(curr->left, command, index - 1);
  for (i = index + string_len; i < command_len; i++)
  {
    if (command[i] == ' ')
      continue;
    else
    {
      index = i;
      break;
    }
  }
  curr->right = malloc((command_len - index + 1) * sizeof(char));
  memset(curr->right, 0, sizeof(curr->right));
  stringcpy(curr->right, command + index, command_len - index + 1);
  printf(1, "%s\n", curr->right);
  return (struct cmnds *)curr;
}

char **genTokens(char *line)
{
  char **tokens = (char **)malloc(10 * sizeof(char *));
  char *token = (char *)malloc(40 * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for (i = 0; i < strlen(line); i++)
  {
    char readChar = line[i];
    if (readChar == ' ' || readChar == '\n' || readChar == '\t')    {
      token[tokenIndex] = '\0';
      if (tokenIndex != 0)
      {
        tokens[tokenNo] = (char *)malloc(40 * sizeof(char));
        strcpy(tokens[tokenNo++], token);
        tokenIndex = 0;
      }
    }
    else
    {
      token[tokenIndex++] = readChar;
    }
  }

  free(token);
  tokens[tokenNo] = 0;
  return tokens;
}

int getcmd(char *buf, int nbuf)
{
  printf(2, "MyShell>");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if (buf[0] == 0)
    return -1;
  return 0;
}

void utilityFunction(char *buf)
{
  char **tokens;

  struct cmnds *token;

  token = malloc(sizeof(*token));

  token->cmndstr = buf;
  // tokens = genTokens(buf);

  token->subcmndlen = strlen("&&");
  token->cmndlen = strlen(buf);

  if (contains(token, "&&")->hasOp == 1)
  {

    printf(1, "%s %d \n", token->left, strlen(token->left));
    printf(1, "%s %d \n", token->right, strlen(token->right));

    printf(1, "\n");
    printf(1, "contains &&\n");
    
    tokens = genTokens(buf);
    exec(tokens[0], tokens);

  }
  else if (contains(token, "||")->hasOp == 1)
  {
    printf(1, "%s %d \n", token->left, strlen(token->left));
    printf(1, "%s %d \n", token->right, strlen(token->right));

    printf(1, "\n");
    printf(1, "contains ||\n");
  }
  else if (contains(token, ";")->hasOp == 1)
  {

    printf(1, "%s %d \n", token->left, strlen(token->left));
    printf(1, "%s %d \n", token->right, strlen(token->right));

    printf(1, "\n");
    printf(1, "contains ;\n");
  }
  else if (contains(token, "|")->hasOp == 1)
  {
    printf(1, "%s %d \n", token->left, strlen(token->left));
    printf(1, "%s %d \n", token->right, strlen(token->right));

    printf(1, "\n");
    printf(1, "contains |\n");
  }
  else
  {
    // its time to directly execute command by calling exec method
    printf(1, "call execute\n");
    tokens = genTokens(buf);
    exec(tokens[0], tokens);
  }
}

int main(void)
{
  static char buf[100];
  int fd;

  // Ensure that three file descriptors are open.
  while ((fd = open("console", O_RDWR)) >= 0)
  {
    if (fd >= 3)
    {
      close(fd);
      break;
    }
  }

  // Read and run input commands.
  while (getcmd(buf, sizeof(buf)) >= 0)
  {
    if (buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
    {
      // Chdir must be called by the parent, not the child.
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

  } //while ends

  exit(0);
}
