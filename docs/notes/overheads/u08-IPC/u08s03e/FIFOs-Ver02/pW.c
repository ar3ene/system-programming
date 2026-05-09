/*
 *  C program to implement one side of FIFO 
 *  This side writes first, then reads 
 *  Version 2: With Error Checkning
 *
 *  Run this program on a shell windows
 */

#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
#include <errno.h>

extern int errno;

int main() { 
  int ret, fd; 
  char str[80]; 
  char * myfifo = "/tmp/myfifo"; 

  ret = mkfifo (myfifo, 0666); 

  // Manually check for error
  if (errno == EEXIST)
    fprintf (stderr, "FIFO exists.\n");
  // Using perror to check for error
  sprintf (str, "Writer (return value=%d)", ret);
  perror (str);

  fd = open(myfifo, O_WRONLY); 

  while (1) { 
    printf ("Send to user: ");
    fgets (str, 80, stdin); 
    write (fd, str, strlen (str)+1); 

    if (strncmp (str, "end", 3)==0) {
      break;
    }
  }

  close (fd); 

  return 0; 
} 
