/* Hate Andrew Shell implemented by Peter Czupil */
/* Design by Doxhound, 2014 */
/* Note: undefined behaviour on Linux machines, run on MINIX */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define MAX_JOBS 15

extern char **get_line(void);
extern char **environ;

struct job{
  long pid;
  char *command;
};

typedef struct job job;

int EXIT_STATUS = EXIT_SUCCESS;
int BKGRND_PROC = 0;
int job_size = 0;

void zombie_handler(int signo){
  int chld_status; 
  if(signo == SIGCHLD){
    wait(&chld_status);
    job_size--;
    printf("Job Finished\n");
   }
  return;
}/* end zombie_handler */

int main(int argc, char *argv[]) {
  FILE *is_error;
  int i, j, chld_status;
  int REDIR_OUT = 0;
  int REDIR_IN = 0;
  int REDIR_PIPE = 0;
  int cmd_size = 0;
  int job_ind = 0;
  long pid;
  char *infile, *outfile, *pipe_in, *pipe_out;
  char *prompt = "[hash-1.0]%"; /* hate ash */
  char **args; 
  char *exec_args[15];/* args to send to execvp */
  job job_array[MAX_JOBS]; /* array of structures to store job list */

  struct sigaction new_action, old_action;
  new_action.sa_handler = zombie_handler;

  /* save old sigchld handler */
  sigaction(SIGCHLD, NULL, &old_action);
  /* install new action */
  sigaction(SIGCHLD, &new_action, NULL);

  /* main loop; reset flags, parse arguments, fork and handle errors */
  while(1) {
    REDIR_IN = 0;
    REDIR_OUT = 0;
    REDIR_PIPE = 0;
    BKGRND_PROC = 0;
    cmd_size = 0;
    
    /* say something nice to the operator */
    printf("%s ", prompt);
    args = get_line();
    if(!args) break;
    if(args[0]  == NULL)
      continue;
    if(!strcmp(args[0], "exit")) exit(EXIT_STATUS);
    
    /* Extra feature: change prompt */
    if(!strcmp(args[0], "prompt") && args[1] != NULL){
      if(!strcmp(args[1], "default")){
        prompt = "[hash-1.0]%";
        continue;
      }
      prompt = *args[1];
      continue;
    }/* end prompt feature */

    /* A loop to parse command line arguments */
    j = 0;
    for(i = 0; args[i] != NULL; i++){
      cmd_size++;
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

    /*create new argument list based on redirection */
    if(REDIR_IN || REDIR_OUT || REDIR_PIPE){
      for(i = 0; i < j; i++){
        exec_args[i] = args[i];
      }
      exec_args[i] = NULL;
    }else{
      for(i = 0; i < cmd_size; i++){
        exec_args[i] = args[i];
      }
      exec_args[i] = NULL;
    }

    /* handle background processes */
    if(BKGRND_PROC){
      pid = fork();
      if(pid == -1){
        printf("Failed to fork()\n");
        continue;
      }
      else if(pid != 0){
        /*parent code*/
        continue;
      }else{
        /*child code*/
        if(REDIR_IN){
          is_error = freopen(infile, "r", stdin);
          if(is_error == NULL){
            EXIT_STATUS = EXIT_FAILURE;
            printf("Could not open file: %s\n", infile);
            continue;
           }
        }/* end if REDIR_IN */
        if(REDIR_OUT){
          is_error = freopen(outfile, "w", stdout);
          if(is_error == NULL){
            EXIT_STATUS = EXIT_FAILURE;
            printf("Could not open file: %s\n", outfile);
            continue;
           }
        }/* end if REDIR_OUT */
        args[cmd_size - 1] = NULL;
        job_size++;
        execvp(args[0], exec_args);
        printf("Could not execute %s\n", args[0]);
        EXIT_STATUS = 1;
      }
    }/* end if & */
    else{
      pid = fork();
      if(pid == -1){
        printf("Failed to fork()\n");
        continue;
      }
      if(pid != 0){
        waitpid(pid, &chld_status, 0);

      }else{
        if(REDIR_IN){
          is_error = freopen(infile, "r", stdin);
          if(is_error == NULL){
            EXIT_STATUS = EXIT_FAILURE;
            printf("Could not open file: %s\n", infile);
            continue;
           }
        }/* end if REDIR_IN */
        if(REDIR_OUT){
          is_error = freopen(outfile, "w", stdout);
          if(is_error == NULL){
            EXIT_STATUS = EXIT_FAILURE;
            printf("Could not open file: %s\n", outfile);
            continue;
           }
        }/* end if REDIR_OUT */
        execvp(args[0], exec_args);
        printf("Could not execute %s\n", args[0]);
        EXIT_STATUS = 1;
      }
    }/* end else !& */
  }/* end inf while */
  return EXIT_STATUS;
}/* end main */
