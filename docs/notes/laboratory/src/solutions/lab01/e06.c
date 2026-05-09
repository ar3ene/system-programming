#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
  int i, j, h, n, pid;

  setbuf (stdout, 0);
  h = atoi (argv[1]);
  n = atoi (argv[2]);

  printf ("    Initial process: pid=%d\n", getpid());
	
  for (i=0; i<h; i++) {
    for (j=0; j<n; j++) {
      pid = fork ();
      if (pid==0) {
        sleep (1);
        break;
      } else {
        sleep (1);
        printf ("    Generation: pid=%d -> pid=%d\n", getpid(), pid);
        if (j==n-1) {
	  printf ("    Exit: pid=%d\n", getpid());
          exit (0);
        }
      }
    }
  }

  printf ("LEAF PROCESS pid=%d\n", getpid());

  return (0);
}

