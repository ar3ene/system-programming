#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

int main () { 
  int i, pid;
  setbuf(stdout,0);
  for (i=1; i<3; i++) { 
    pid = fork ();
    i++;
    if (pid!=0) { 
      fork();
      printf("X");
    } 
  } 
  printf("X"); 
  return 1; 
} 
