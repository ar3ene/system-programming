/*
 *  StQ 20.05.2024
 *  POSIX Async IO (with signal)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <aio.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 128

struct aiocb aio_cb;

void aio_completion_handler(int sig, siginfo_t *si, void *context) {
    if (sig == SIGUSR1) {
	printf("Received SIGUSR1; ");	            
        // Check if the asynchronous read completed successfully
        if (aio_error(&aio_cb) == 0) {
            // Get the number of bytes read
            ssize_t bytes_read = aio_return(&aio_cb);
            if (bytes_read > 0) {
                // Null-terminate the buffer
	        char *str;
		str = (char *) aio_cb.aio_buf;
	        str[bytes_read-1] = '\0';		
	        //aio_cb.aio_buf[bytes_read] = '\0';
                // Print the input received
		printf("Received input: %s\n", str);
		if (strcmp(str, "end")==0) {
		  exit (1);
		}
            } else {
                printf("No input received\n");
            }
        } else {
            perror("aio_error");
        }
    }
}

int main() {
    struct sigaction sa;
    char buffer[BUFFER_SIZE];

    setbuf (stdout, 0);
    
    // Set up the signal handler for SIGUSR1
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = aio_completion_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Initialize the aiocb structure
    memset(&aio_cb, 0, sizeof(struct aiocb));
    aio_cb.aio_fildes = STDIN_FILENO;
    aio_cb.aio_buf = buffer;
    aio_cb.aio_nbytes = BUFFER_SIZE - 1;
    aio_cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    aio_cb.aio_sigevent.sigev_signo = SIGUSR1;

    // Initiate the first asynchronous read
 
    // Main loop
    while (1) {
      // Initiate the asynchronous read
      if (aio_read(&aio_cb) == -1) {
        perror("aio_read");
        exit(EXIT_FAILURE);
      }
      printf("Waiting for input ... ");
      // Suspend the process until a signal is received
      pause();
    }

    return 0;
}
