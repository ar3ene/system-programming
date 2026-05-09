#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void sigUsr (int);

int s1, s2, s3;
int n = 0;

static void
sigUsr (
  int signo
) {
  n++;
  s1 = s2;
  s2 = s3;

  if (signo == SIGUSR1)
    s3 = 1;

  if (signo == SIGUSR2)
    s3 = -1;

  // Only for First Call
  if (n==1)
    return;

  if (s1==s2 && s2==s3) {
    fprintf (stdout, "Stop ...\n");
    exit (1);
  }

  if (s2==s3)
    fprintf (stdout, "Error ...\n");

  if (s2!=s3)
    fprintf (stdout, "Success ...\n");

  return;
}

int
main (void) {
  if (signal(SIGUSR1, sigUsr) == SIG_ERR) {
    fprintf (stderr, "Signal Handler Error.\n");
    return (1);
  }
  if (signal(SIGUSR2, sigUsr) == SIG_ERR) {
    fprintf (stderr, "Signal Handler Error.\n");
    return (1);
  }

  while (1) {
    //fprintf (stdout, "Before pause.\n");
    pause ();
    //fprintf (stdout, "After pause.\n");
  }

  return (0);
}
