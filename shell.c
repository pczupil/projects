#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#undef _POSIX_
#define _POSIX_
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

extern char **get_line(void);
extern char **environ;

int EXIT_STATUS = EXIT_SUCCESS;

int main(int argc, char *argv[]) {
  int i;
  int chld_status;
  int pid;
  char *out_redir, *in_redir, *pipe, *amper;
  char *prompt = "hash1.0%";
  char **args; 

  while(1) {
    printf("%s ", prompt);
    args = get_line();
    if(!args) break;
    if(strcmp(args[0], "exit") == 0) exit(EXIT_STATUS);
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
