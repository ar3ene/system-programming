#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define N 100
#define L 255

typedef struct record_s {
  long int n;
  char s[N];
  float f;
  struct record_s *next;
} record_t;

char skip_spaces (int);
  
int main (int argc, char **argv) {
  FILE *fp;
  char line[L], tmp1[N], tmp2[N], c;
  int i, fd, not_end;
  long int n;
  float f;
  record_t *r, *p, *head;

  //
  // Read C 1
  //

  head = NULL;
  fp = fopen (argv[1], "r");
  while (fgets (line, L, fp) != NULL) {
    r = (record_t *) malloc (1 * sizeof (record_t)); 
    sscanf (line, "%ld %s %f", &r->n, r->s, &r->f);
    r->next = head;
    head = r;
  }
  fclose (fp);

  printf ("Read C 1\n");
  for (p=head; p!=NULL; p=p->next) {
    printf ("%ld %s %f\n", p->n, p->s, p->f);
  }

  // Free list, etc.

  //
  // Read C 2
  //

  head = NULL;
  fp = fopen (argv[1], "r");
  while (fscanf (fp, "%ld %s %f", &n, tmp1, &f) != EOF) {
    r = (record_t *) malloc (1 * sizeof (record_t)); 
    r->n = n; strcpy (r->s, tmp1); r->f = f;
    r->next = head;
    head = r;
  }
  fclose (fp);

  printf ("Read C 2\n");
  for (p=head; p!=NULL; p=p->next) {
    printf ("%ld %s %f\n", p->n, p->s, p->f);
  }

  // Free list, etc.
  
  //
  // Read POSIX 1
  //

  fd = open (argv[1], O_RDONLY);
  head = NULL;
  do {
    i = 0;
    do {  
      not_end = read (fd, &c, sizeof(char));
      if (not_end != 0) {
        line[i++] = c;
      }
    } while (c!='\n' && not_end);
    if (not_end) {
      r = (record_t *) malloc (1 * sizeof (record_t)); 
      sscanf (line, "%ld %s %f", &r->n, r->s, &r->f);
      r->next = head;
      head = r;
    }
  } while (not_end);
  close (fd);

  printf ("Read Posix 1\n");
  for (p=head; p!=NULL; p=p->next) {
    printf ("%ld %s %f\n", p->n, p->s, p->f);
  }

  //
  // Read POSIX 2
  //
  
  fd = open (argv[1], O_RDONLY);
  head = NULL;
  do {
    n = 0;
    do {  
      not_end = read (fd, &c, sizeof(char));
      if (c!=' ' && not_end) {
        n = n * 10 + ((int) (c-'0'));
      }
    } while (c!=' ' && not_end);

    c = skip_spaces (fd);
    i = 0;
    tmp1[i++] = c;    
    do {  
      not_end = read (fd, &c, sizeof(char));
      if (c!=' ' && not_end) {
        tmp1[i++] = c;
      }
    } while (c!=' ' && not_end);
    tmp1[i] = '\0';

    skip_spaces (fd);
    i = 0;
    tmp2[i++] = c;
    do {  
      not_end = read (fd, &c, sizeof(char));
      if (c!=' ' && not_end) {
        tmp2[i++] = c;
      }
    } while (c!='\n' && not_end);

    if (not_end) {
      r = (record_t *) malloc (1 * sizeof (record_t)); 
      r->n = n;
      strcpy (r->s, tmp1);
      r->f = atof (tmp2);
      r->next = head;
      head = r;
    }
  } while (not_end);
  close (fd);

  printf ("Read Posix 2\n");  
  for (p=head; p!=NULL; p=p->next) {
    printf ("%ld %s %f\n", p->n, p->s, p->f);
  }

  return (1);
}

char skip_spaces (int fd) {
  char c;
  
  do {
    read (fd, &c, sizeof(c));
  } while (c==' ');

  return c;
}
