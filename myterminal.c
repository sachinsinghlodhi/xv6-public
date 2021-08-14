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

// void
// runcmd(struct cmd *cmd)
// {
//   int p[2];
//   struct backcmd *bcmd;
//   struct execcmd *ecmd;
//   struct listcmd *lcmd;
//   struct pipecmd *pcmd;
//   struct redircmd *rcmd;

//   if(cmd == 0)
//     exit(0);

//   switch(cmd->type){
//   default:
//     panic("runcmd");

//   case EXEC:
//     ecmd = (struct execcmd*)cmd;
//     if(ecmd->argv[0] == 0)
//       exit(0);
//     exec(ecmd->argv[0], ecmd->argv);
//     printf(2, "exec %s failed\n", ecmd->argv[0]);
//     break;

//   case REDIR:
//     rcmd = (struct redircmd*)cmd;
//     close(rcmd->fd);
//     if(open(rcmd->file, rcmd->mode) < 0){
//       printf(2, "open %s failed\n", rcmd->file);
//       exit(0);
//     }
//     runcmd(rcmd->cmd);
//     break;

//   case LIST:
//     lcmd = (struct listcmd*)cmd;
//     if(fork1() == 0)
//       runcmd(lcmd->left);
//     wait();
//     runcmd(lcmd->right);
//     break;

//   case PIPE:
//     pcmd = (struct pipecmd*)cmd;
//     if(pipe(p) < 0)
//       panic("pipe");
//     if(fork1() == 0){
//       close(1);
//       dup(p[1]);
//       close(p[0]);
//       close(p[1]);
//       runcmd(pcmd->left);
//     }
//     if(fork1() == 0){
//       close(0);
//       dup(p[0]);
//       close(p[0]);
//       close(p[1]);
//       runcmd(pcmd->right);
//     }
//     close(p[0]);
//     close(p[1]);
//     wait();
//     wait();
//     break;

//   case BACK:
//     bcmd = (struct backcmd*)cmd;
//     if(fork1() == 0)
//       runcmd(bcmd->cmd);
//     break;

//   case EXIT:
//     // exitcall = (struct exitcmd*)cmd;
//     exit(0);
 
//   }
//   exit(0);
// }

int
getcmd(char *buf, int nbuf)
{
  printf(2, "myshell> ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
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
      struct struct_command *token ;

      token = malloc(sizeof(*token));

      token->ptrforcommand = buf;
      
      // pattern will be passed as parameter
      // token->ptrforcheckpattern = "&&";

      token ->subcommandLength =  strlen("&&");
      token->commandLength = strlen(buf);
      

      if(contains(token , "&&")->exist == 1){

          printf(1,"%s %d \n",token->leftPart, strlen(token->leftPart));
          printf(1,"%s %d \n",token->rightPart, strlen(token->rightPart));

          printf(1,"\n");
          printf(1,"contains &&\n");

      }else if(contains(token , "||")->exist == 1){

        printf(1,"%s %d \n",token->leftPart, strlen(token->leftPart));
          printf(1,"%s %d \n",token->rightPart, strlen(token->rightPart));

          printf(1,"\n");
          printf(1,"contains &&\n");

    }else if(contains(token , ";")->exist == 1){

       printf(1,"%s %d \n",token->leftPart, strlen(token->leftPart));
          printf(1,"%s %d \n",token->rightPart, strlen(token->rightPart));

          printf(1,"\n");
          printf(1,"contains &&\n");

    }else if(contains(token , "|")->exist == 1){

       printf(1,"%s %d \n",token->leftPart, strlen(token->leftPart));
          printf(1,"%s %d \n",token->rightPart, strlen(token->rightPart));

          printf(1,"\n");
          printf(1,"contains &&\n");

    }else{
        // its time to directly execute command by calling exec method
        printf(1,"call ececute\n");

    }
     
    }
    wait(0);

  }//while ends

  exit(0);
}
