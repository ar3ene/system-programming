#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
  char str[20];
  int n = atoi (argv[1]);

  setbuf (stdout, 0);
  if (n<=0) return (1);
  printf ("run with n=%d\n", n);
  if (fork()>0) {
    sprintf (str, "%d", n-1);
    printf ("exec\n");
    execlp (argv[0], argv[0], str, (char *) 0);
  } else {
    sprintf (str, "%s %d", argv[0], n-2);
    printf ("system\n");
    system (str);
  }
  wait ((int *) 0);

  return (1);
}
