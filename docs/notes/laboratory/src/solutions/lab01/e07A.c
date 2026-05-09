/*
 *  Print-out binary numbers with concurrency
 *  No order is imposed
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void binary(int *, int);

int
main (
  int argc,
  char *argv[]
  )
{
  int n;
  int *vet;

  if (argc<2) {
    fprintf (stderr, "Run  it as: pgrm n\n");
    exit (1);
  }

  n = atoi (argv[1]);

  vet = (int *)malloc(n * sizeof(int));
  if (vet == NULL) {
    printf("Allocatin Error.\n");
    exit(1);
  }

  printf("Binary Numbers:\n");
  binary(vet, n);
  free(vet);

  return 0;
}

void
binary (
  int *vet,
  int n
  )
{
  int i, j;
  pid_t pid1, pid2;

  for (i=0; i<=n; i++) {
    if (i == n) {

      for (j=0; j<n; j++) {
        printf("%d", vet[j]);
      }
      printf("\n");
      exit (0);

    } else {


      pid1 = fork ();       
      if (pid1 == 0) {
        // Child 1
        vet[i] = 0;
      } else {
        // Father
        pid2 = fork ();       
        if (pid2 == 0) {
          // Child 2
          vet[i] = 1;
        } else {
          wait ((int *) 0);
          wait ((int *) 0);
          exit (1);
        }
      }
 
    } // if
  }  // for

  exit (0);
}



