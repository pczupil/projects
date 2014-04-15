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
  int i;
  int chld_status;
  int pid;
  int REDIR_OUT = 0;
  int REDIR_IN = 0;
  int REDIR_PIPE = 0;
  int BKGRND_PROC = 0;
  char *infile, *outfile;
  char *prompt = "hash1.0%";
  char **args; 

  while(1) {
    printf("%s ", prompt);
    args = get_line();
    if(!args) break;
    if(strcmp(args[0], "exit") == 0) exit(EXIT_STATUS);

    for(i = 0; args[i] != NULL; i++){
      printf("%s\n", args[i]);
      if(strcmp("&", args[i]) == 0){
          BKGRND_PROC = 1;
          break;
      }
      if(strcmp("<", args[i]) == 0){
          REDIR_IN = 1;
          *infile = *args[i+1];
          break;
      }
      if(strcmp(">", args[i]) == 0){
          REDIR_OUT = 1;
          *outfile = *args[i+1];
          break;
      }
      if(strcmp("|", args[i]) == 0){
          REDIR_PIPE = 1;
          *infile = *args[i-1];
          *outfile = *args[i+1];
          break;
      }
    }/* end for */
    pid = fork();
    if(pid != 0){
      /*parent code*/
      wait(&chld_status);
    }else{
      /*child code*/
      execvp(args[0], args);
      printf("Could not execute %s\n", args[0]);
    }
  }
  return 0;
}
