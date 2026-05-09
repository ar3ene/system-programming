#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char ** argv) {
    int l, fd[2];
    char s[100];

    // no buffering
    setbuf (stdout, 0);
    fprintf (stdout, "Pipe example (C):\n");
    
    // Create pipe
    pipe (fd);

    if (fork()!=0) {
        while (1) {
            fprintf (stdout, "  Father Write (string, 'end' to stop): ");
            scanf ("%s", s);
            l = strlen (s) + 1;
            write (fd[1], &l, sizeof(int));
            write (fd[1], s, l*sizeof(char));
            if (strcmp (s, "end")==0) break;
	    sleep (1);
        }
        wait ((int *) 0);
    } else {
        while (1) {
            read (fd[0], &l, sizeof(int));
            read (fd[0], s, l*sizeof(char));
            fprintf (stdout, "  Child Read: %s\n", s);
            if (strcmp (s, "end")==0) break;
        }
        exit (0);
    }

    return (0);
}
