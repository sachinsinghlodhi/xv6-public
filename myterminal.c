// Shell.

#include "types.h"
#include "user.h"
#include "fcntl.h"
// #include "string.h"

struct struct_command{
    
    int exist; // to check if | && || ; exist
    int type;

    int commandLength;
    int subcommandLength;
    
    // char *ptrforcheckpattern;
    char *ptrforcommand;
    char  *leftPart;
    char *rightPart;
    
};


int
strlength(char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

int
strncopy(char *s, const char *t, int n)
{
 
  while(n-- > 0 && (*s++ = *t++) != 0)
    ;
  while(n-- > 0)
    *s++ = 0;
  return 0;
}

struct struct_command* contains(struct struct_command *parameter , char* ptrforcheckpattern){
    int i = 0,exist = 0 , index = 0;//0 denotes false
    int command_len = parameter->commandLength , string_len = strlength(ptrforcheckpattern) ;
    char *command = parameter->ptrforcommand , *ptr = ptrforcheckpattern;

    for(i = 0;i < command_len - string_len;i++){
         exist = 0;
         for(int j = 0;j < string_len;j++){
            //  printf("%c %c",command[i] , ptr[j]);
             if(ptr[j] != command[i+j]){

                 exist = 1;
                 break;

             }

         }
        //  printf("\n");
         if(exist == 0){
             index = i;
             break;
         }
    }

    if(index == 0){
    
        parameter->exist = 0;
        return (struct struct_command*)parameter;

    }




    // printf("%d\n",index);
    parameter->exist = 1;

     parameter->leftPart = malloc((index)*sizeof(char));
     memset(parameter->leftPart,0,sizeof(parameter->leftPart));
    // right = malloc((command_len - 1 - index - 1)*(sizeof(char)));

    // char **pointer = &left , **print = &left;

     strncopy(parameter->leftPart,command,index-1);
    // parameter->leftPart[index] = '\0';

    // printf("%d\n",index + parameter->subcommandLength);

    for(i = index + string_len ; i < command_len;i++){
        if(command[i] == ' ')continue;
        else{
            index = i;
            break;
        }
    }

    // printf("%s\n",command+index);
    //  strncpy(parameter->rightPart,command+index,command_len - index);
   
   parameter->rightPart = malloc( (command_len - index + 1)*sizeof(char));
    memset(parameter->rightPart,0,sizeof(parameter->rightPart));
    
     strncopy(parameter->rightPart,command+index,command_len - index + 1);

    printf(1,"%s\n",parameter->rightPart);

    return (struct struct_command*)parameter;
}

char **tokenize(char *line)
{
  char **tokens = (char **)malloc(10 * sizeof(char *));
  char *token = (char *)malloc(40 * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(40*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0;
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }

  free(token);
  tokens[tokenNo] = 0 ;
  return tokens;
}

int
getcmd(char *buf, int nbuf)
{
  printf(2, "MyShell>");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0)
    return -1;
  return 0;
}

void utilityFunction(char *buf)
{
  char **tokens;

  struct struct_command *token;

  token = malloc(sizeof(*token));

  token->ptrforcommand = buf;
  tokens = tokenize(buf);

  token->subcommandLength = strlen("&&");
  token->commandLength = strlen(buf);

  if (contains(token, "&&")->exist == 1)
  {

    printf(1, "%s %d \n", token->leftPart, strlen(token->leftPart));
    printf(1, "%s %d \n", token->rightPart, strlen(token->rightPart));

    printf(1, "\n");
    printf(1, "contains &&\n");
  }
  else if (contains(token, "||")->exist == 1)
  {

    printf(1, "%s %d \n", token->leftPart, strlen(token->leftPart));
    printf(1, "%s %d \n", token->rightPart, strlen(token->rightPart));

    printf(1, "\n");
    printf(1, "contains ||\n");
  }
  else if (contains(token, ";")->exist == 1)
  {

    printf(1, "%s %d \n", token->leftPart, strlen(token->leftPart));
    printf(1, "%s %d \n", token->rightPart, strlen(token->rightPart));

    printf(1, "\n");
    printf(1, "contains ;\n");
  }
  else if (contains(token, "|")->exist == 1)
  {
    printf(1, "%s %d \n", token->leftPart, strlen(token->leftPart));
    printf(1, "%s %d \n", token->rightPart, strlen(token->rightPart));

    printf(1, "\n");
    printf(1, "contains |\n");
  }
  else
  {
    // its time to directly execute command by calling exec method
    printf(1, "call execute\n");
    exec(tokens[0], tokens);
  }
}



int
main(void)
{
  static char buf[100];
  int fd;


  // Ensure that three file descriptors are open.
  while((fd = open("console", O_RDWR)) >= 0){
    if(fd >= 3){
      close(fd);
      break;
    }
  }

  // Read and run input commands.
  while(getcmd(buf, sizeof(buf)) >= 0){
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
      // Chdir must be called by the parent, not the child.
      buf[strlen(buf)-1] = 0;  // chop \n
      if(chdir(buf+3) < 0)
        printf(2, "cannot cd %s\n", buf+3);
      continue;
    }


    if(fork() == 0){
      utilityFunction(buf);     
    }
    wait(0);

  }//while ends

  exit(0);
}
