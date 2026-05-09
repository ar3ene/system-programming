#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main() { 
  int fd1, fd2; 
  char str1[80], str2[80]; 
  char *myfifo1 = "/tmp/myfifo1";
  char *myfifo2 = "/tmp/myfifo2"; 

  mkfifo (myfifo1, 0666);
  mkfifo (myfifo2, 0666); 
  fd1 = open(myfifo1, O_WRONLY);
  fd2 = open(myfifo2, O_WRONLY); 
    
  while (1) { 
    printf ("To send to user1: ");
    fgets (str1, 80, stdin); 
    write (fd1, str1, strlen (str1)+1); 

    printf ("To send to user2: ");
    fgets (str2, 80, stdin); 
    write (fd2, str2, strlen (str2)+1); 

    if (strncmp (str1, "end", 3)==0 && strncmp (str2, "end", 3)==0) {
      break;
    }
  }

  close (fd1);
  close (fd2);
      
  return 0; 
} 
