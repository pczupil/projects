#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#define C_POSIX_SOURCE 200809L
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

extern char **get_line(void);
extern char **environ;

int EXIT_STATUS = EXIT_SUCCESS;

int main(int argc, char *argv[]) {
  int i, j, chld_status, pid;
  int cmd_size = 0;
  int REDIR_OUT = 0;
  int REDIR_IN = 0;
  int REDIR_PIPE = 0;
  int BKGRND_PROC = 0;
  char *infile, *outfile, *pipe_in, *pipe_out;
  char *prompt = "hash1.0%";
  char **args; 
  char *exec_args[15];/* args to send to execvp */

  while(1) {
    REDIR_IN = 0;
    REDIR_OUT = 0;
    REDIR_PIPE = 0;
    BKGRND_PROC = 0;
    cmd_size = 0;
    printf("%s ", prompt);
    args = get_line();
    if(!args) break;
    if(args[0]  == NULL)
      continue;
    if(!strcmp(args[0], "exit")) exit(EXIT_STATUS);

    /* A loop to parse command line arguments */
    j = 0;
    for(i = 0; args[i] != NULL; i++){
      cmd_size++;
      fflush(stdout);
      if(!strcmp("&", args[i])){
          BKGRND_PROC = 1;
          break;
      }else if(!strcmp("<", args[i])){
          REDIR_IN = 1;
          infile = args[i+1];
          j = i;
          i+2;
          continue;
      }else if(!strcmp(">", args[i])){
          REDIR_OUT = 1;
          outfile = args[i+1];
          j = i;
          i+2;
          continue;
      }else if(!strcmp("|", args[i])){
          REDIR_PIPE = 1;
          pipe_in = args[i-1];
          pipe_out = args[i+1];
          continue;
      }
    }/* end for */
    printf("Infile: %s, Outfile: %s\n", infile, outfile);
    if(REDIR_IN || REDIR_OUT || REDIR_PIPE){
      for(i = 0; i < j; i++){
        exec_args[i] = args[i];
        printf("exec_args: %s\n", exec_args[i]);
      }
      exec_args[i] = NULL;
    }else{
      for(i = 0; i < cmd_size; i++){
        exec_args[i] = args[i];
      }
      exec_args[i] = NULL;
    }

    if(BKGRND_PROC){
      pid = fork();
      if(pid != 0){
        /*parent code*/
        continue;
      }else{
        /*child code*/
        if(REDIR_IN)
          freopen(infile, "r", stdin);
        if(REDIR_OUT)
          freopen(outfile, "w", stdout);
        args[cmd_size - 1] = NULL;
        execvp(args[0], exec_args);
        printf("Could not execute %s\n", args[0]);
        EXIT_STATUS = 1;
      }
    }/* end if & */
    else{
      pid = fork();
      if(pid != 0){
        wait(&chld_status);

      }else{
        if(REDIR_IN)
          freopen(infile, "r", stdin);
        if(REDIR_OUT)
          freopen(outfile, "w", stdout);
        execvp(args[0], exec_args);
        printf("Could not execute %s\n", args[0]);
        EXIT_STATUS = 1;
      }
    }/* end else !& */
  }/* end inf while */
  return EXIT_STATUS;
}/* end main */
