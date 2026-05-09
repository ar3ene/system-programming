// C program to implement one side of FIFO 
// This side reads first, then reads 

#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main() {
  int fd1; 
  char str[80]; 
  char *myfifo = "/tmp/myfifo"; 

  mkfifo (myfifo, 0666); 
  fd1 = open (myfifo, O_RDONLY); 

  while (1) { 
      read (fd1, str, 80); 
      printf ("Received from user1: %s", str); 

      if (strncmp (str, "end", 3)==0) {
	break;
      }
  } 

  close(fd1); 

  return 0; 
} 
