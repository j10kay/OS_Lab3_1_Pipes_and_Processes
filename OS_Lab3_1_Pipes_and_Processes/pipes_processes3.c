#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h> 

int main(int argc, char **argv)
{
  int pipefd[2];
  int pipefd2[2];
  int pid, pid2;

   char *cat_args[] = {"cat", "scores", NULL};
   // char *grep_args[] = {"grep", "Lakers", NULL};

  if( argc == 2 ) {
      printf("The argument supplied is %s\n", argv[1]);
   }
   else if( argc > 2 ) {
      printf("Error! Too many arguments supplied.\n");
   }
   else {
      printf("Error! One argument expected but none was given.\n");
   }
  
  // make a pipe (fds go in pipefd[0] and pipefd[1])

    pipe(pipefd);
    pipe(pipefd2);

    pid = fork();
  
    if (pid < 0){
      
        fprintf(stderr, "fork Failed" ); 
        return 1; 
        
    } 
    else if (pid == 0)
    {
      
        pid2 = fork();
        if (pid2 < 0) {
          fprintf(stderr, "fork Failed" ); 
          return 1; 
        }
        else if (pid2 == 0){

           dup2(pipefd2[0], 0);
          close(pipefd2[1]);
          close(pipefd[1]);
          close(pipefd[0]);
          // execute sort
          char *sort_input[] = {"sort", NULL}; 
          execvp("sort", sort_input);
          close(pipefd2[0]);
        }
        else {
          dup2(pipefd[0], 0);
          dup2(pipefd2[1], 1);
          
          close(pipefd[1]);
          close(pipefd2[0]);
          // execute grep
          execvp("grep", argv);

          close(pipefd[1]);
          close(pipefd2[1]);
        }

    }
    else
    {
        // parent gets here and handles "cat scores"

        // replace standard output with output part of pipe

        dup2(pipefd[1], 1);

        // close unused unput half of pipe

        close(pipefd[0]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        // execute cat
        execvp("cat", cat_args);
        
    }
}