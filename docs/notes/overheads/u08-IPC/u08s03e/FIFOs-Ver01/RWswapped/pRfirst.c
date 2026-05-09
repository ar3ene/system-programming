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
  // FIFO file path 
  char *myfifo = "/tmp/myfifo"; 

  // Creating the named file(FIFO) 
  // mkfifo(<pathname>,<permission>) 
  mkfifo (myfifo, 0666); 

  while (1) { 
      // First open in read only and read 
      fd1 = open (myfifo, O_RDONLY); 
      read (fd1, str, 80); 
      printf ("Received from user1: %s", str); 
      close(fd1); 

      if (strncmp (str, "end", 3)==0) {
	break;
      }

      // Now open in write mode and write 
      fd1 = open (myfifo, O_WRONLY); 
      printf ("To send to user1: ");
      fgets (str, 80, stdin); 
      write (fd1, str, strlen (str)+1); 
      close (fd1); 

      if (strncmp (str, "end", 3)==0) {
        break;
      }
  }
  
  return 0; 
} 
