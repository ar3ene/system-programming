#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
  int i, j, n;
  int *vet;
  vet = (int *)malloc(n * sizeof(int));
  if (vet == NULL) {
    printf("Allocation Error.\n");
    exit(1);
  }
  printf("Binary Numbers:\n");

  n = atoi (argv[1]);

  for (i=0; i<n; i++)  {
    if (fork())
      vet[i] = 1;
    else vet[i] = 0;
  }
  
  for (j=0; j<n; j++) {
    printf("%d", vet[j]);
  }
  printf("\n");


  return (0);
}
