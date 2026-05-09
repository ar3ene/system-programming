#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define N 132

void parent (char *, int *, int *);
void child (char *, int *, int *);
void read_file (char *);
int write_file (char *);

int main (int argc, char **argv) {
  int fd1[2], fd2[2];
  int childPid;
  pipe (fd1);
  pipe (fd2);
  childPid = fork();
  if (childPid == 0) {
    child (argv[1], fd1, fd2);   
  } else {
    parent (argv[1], fd1, fd2);   
  }
  wait ((void *)0); 
  exit (1);
}

void  parent (char *name, int *fd1, int *fd2) {
  char c;
  int stop = 0;
  close (fd1[0]); close (fd2[1]); 
  while (stop==0) {
    printf ("PARENT WAKE UP (PID=%d) \n", getpid());
    read_file (name);
    stop = write_file (name);
    c = (stop==0) ? '0' : '1';
    write (fd1[1], &c, sizeof (char));
    if (stop==1)
      break;
    read (fd2[0], &c, sizeof (char));
    stop = (c=='0') ? 0 : 1;
  }
  close (fd1[1]); close (fd2[0]);
  return;
}

void child (char *name, int *fd1, int *fd2) {
  char c;
  int stop = 0;
  close (fd1[1]); close (fd2[0]);
  while (stop==0) {    
    read (fd1[0], &c, sizeof (char));
    printf ("CHILD WAKE UP (PID=%d) \n", getpid());    
    if (c=='1')
      break;
    read_file (name);
    stop = write_file (name);
    c = (stop==0) ? '0' : '1';
    write (fd2[1], &c, sizeof (char));
  }
  close (fd1[0]); close (fd2[1]);
  return;
}

void read_file (char *name) {
  FILE *fp;
  char str[N];
  
  fp = fopen (name, "r");
  while (fscanf (fp, "%s", str) != EOF) {
    fprintf (stdout, "   %s\n", str);
  }
  fclose (fp);

  return;
}

int write_file (char *name) {
  FILE *fp;
  char str[N];
  fp = fopen (name, "w");
  do {
    fprintf (stdout, "   str: ");
    scanf ("%s", str);
    if (strcmp (str, "end") != 0 && strcmp (str, "END") != 0) {
      fprintf (fp, "   %s\n", str);
    }
  } while (strcmp (str, "end") != 0 && strcmp (str, "END") != 0);
  fclose (fp);
  if (strcmp (str, "END") == 0)
    return 1;
  else
    return 0;
}





