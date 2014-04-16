Peter Czupil - 1317993 - pczupil@ucsc.edu
CMPS_111 - Spring 2014
Project 1 - Shell
README.md

Files contained in this project:
README.md:
This is the file you're reading right now (hopefully). Describes various files for the shell project.

shell.c:
Implementation file for the shell project. Works in conjunction with lex.yy.c to parse command arguments. It then forks to execute programs as a separate process (processes are identical after fork but have different pids). Features implemented: exit, commands with/without arguments, background processes, input/output redirection and ability to change prompt string using prompt {new_prompt} command.

lex.c:
parses input

Makefile:
use make all to create project. Use ./shell to run program.

