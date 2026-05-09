/// C program to implement one side of FIFO 
// This side writes first, then reads 

#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main() { 
  int fd; 
  char str[80]; 
  char * myfifo = "/tmp/myfifo"; 

  mkfifo (myfifo, 0666); 
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
