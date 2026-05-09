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
  // FIFO file path 
  char * myfifo = "/tmp/myfifo"; 

  // Creating the named file(FIFO) 
  // mkfifo(<pathname>, <permission>) 
  mkfifo (myfifo, 0666); 

  while (1) { 
    // Open FIFO for write only 
    fd = open(myfifo, O_WRONLY); 
    printf ("To send to user2: ");
    fgets (str, 80, stdin); 
    write (fd, str, strlen (str)+1); 
    close (fd); 

    if (strncmp (str, "end", 3)==0) {
      break;
    }

    // Open FIFO for Read only 
    fd = open (myfifo, O_RDONLY); 
    read (fd, str, sizeof(str)); 
    printf ("Received from user2: %s", str);
    close(fd); 

    if (strncmp (str, "end", 3)==0) {
      break;
    }
  } 
  return 0; 
} 
