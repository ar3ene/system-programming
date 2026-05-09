#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void p1(int, int);
static void p2(int, int);
static void p3(int, int);

int main () {
  int p12[2], p23[2], p31[2];

  setbuf (stdout, 0);
  pipe (p12); pipe (p23); pipe (p31);
  if (fork()!=0) {
    close(p12[0]); close(p23[0]); close(p23[1]); close(p31[1]);
    p1 (p31[0], p12[1]);
  } else {
    if (fork()!=0) {
      close(p12[1]); close(p23[0]); close(p31[0]); close(p31[1]);      
      p2 (p12[0], p23[1]);
    } else {
      close(p12[0]); close(p12[1]); close(p23[1]); close(p31[0]);
      p3 (p23[0], p31[1]);      
    }
  }
  return (0);
}

static void p1 (int pr, int pw) {
  int n;
  n = 0;
  while (1) {
    fprintf (stdout, "P1 waiting: %d secs\n", n);    
    sleep (n);
    fprintf (stdout, "P1 reading: ");
    scanf ("%d", &n);
    write (pw, &n, sizeof (int));
    read (pr, &n, sizeof (int));
  }
  return;
}

static void p2 (int pr, int pw) {
  int n;
  while (1) {
    read (pr, &n, sizeof (int));
    fprintf (stdout, "P2 waiting: %d secs\n", n);    
    sleep (n);
    fprintf (stdout, "P2 reading: ");
    scanf ("%d", &n);
    write (pw, &n, sizeof (int));
  }
  return;
}

static void p3 (int pr, int pw) {
  int n;
  while (1) {
    read (pr, &n, sizeof (int));
    fprintf (stdout, "P3 waiting: %d secs\n", n);    
    sleep (n);
    fprintf (stdout, "P3 reading: ");
    scanf ("%d", &n);
    write (pw, &n, sizeof (int));
  }
  return;
}



