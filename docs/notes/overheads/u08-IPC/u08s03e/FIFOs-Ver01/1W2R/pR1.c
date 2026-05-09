#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main() {
  int fd; 
  char str[80]; 
  char *myfifo = "/tmp/myfifo1"; 

  mkfifo (myfifo, 0666); 
  fd = open (myfifo, O_RDONLY); 

  while (1) { 
      read (fd, str, 80); 
      printf ("Received from user1: %s", str); 

      if (strncmp (str, "end", 3)==0) {
	break;
      }
  } 

  close(fd); 

  return 0; 
} 
