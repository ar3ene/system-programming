#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int last_sig = -1; 
int last_last_sig = -1; 
int finish = 0;

 
void sign_handler(int sig){ 
  if (sig==SIGUSR1) 
    printf("Signal received from P1\n"); 
  else if (sig==SIGUSR2) 
    printf("Signal received from P2\n"); 
  if (sig == last_sig && last_sig == last_last_sig) { 
    finish = 1; 
  } else { 
    last_last_sig = last_sig; 
    last_sig = sig; 
  } 
}

int main() { 
  int pid1, pid2;
  char cmd[100]; 

  if ( (signal(SIGUSR1, sign_handler) == SIG_ERR)
    || (signal(SIGUSR2, sign_handler) == SIG_ERR) ) { 
    printf("Error initializing signal handler"); 
    exit(-1); 
  }
  pid1 = fork();
  if (pid1==0) { 
    while (1) {     
      sleep(rand()%4); 
      kill(getppid(), SIGUSR1); 
    } 
  } else { 
    pid2 = fork();
    if (pid2==0) { 
      while (1) { 
        sleep(rand()%5); 
        kill(getppid(), SIGUSR2); 
      } 
    } else {
      while (1) { 
        pause(); 
        if (finish) { 
          sprintf(cmd, "kill -9 %d", pid1); system(cmd); 
          sprintf(cmd, "kill -9 %d", pid2); system(cmd); 
          exit(0); 
        } 
      } 
    }
  }
  return (0);
}

