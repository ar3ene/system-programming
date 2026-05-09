/*
 *  C program to implement one side of FIFO 
 *  This side reads first, then reads
 *  Version 2: With Error Checkning
 *
 *  Run this program on another shell windows
 */

#include <stdio.h>
#include <stdlib.h> 
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
  char *myfifo = "/tmp/myfifo"; 

  ret = mkfifo (myfifo, 0666);

  // Manually check for error
  if (errno == EEXIST)
    fprintf (stderr, "FIFO exists.\n");
  // Using perror to check for error
  sprintf (str, "Reader (return value=%d)", ret);
  perror (str);
  
  fd = open (myfifo, O_RDONLY); 

  while (1) { 
      read (fd, str, 80); 
      printf ("Received from user1: %s", str); 

      if (strncmp (str, "end", 3)==0) {
	break;
      }
  } 

  sprintf (str, "rm -rf %s", myfifo);
  system (str);
  
  close(fd); 

  return 0; 
} 
