#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

// Shared memory segment size: 1K versus 24 bytes (to debug)
// #define SHM_SIZE 1024
#define SHM_SIZE 24

void parent (int [], int [], key_t);
void child (int [], int [], key_t);
int generate (int, char *);
void convert_and_display (char *, int);

int main (int argc, char **argv) {
    key_t key;
    int fd1[2], fd2[2], childPid;

//    setbuf (stdout, 0);

    /* Make the key: The file must exist */
    if ((key = ftok("hello.txt", 3)) == -1) {
        cerr << "Error in ftok." << endl;
        exit (1);
    }

    pipe (fd1);
    pipe (fd2);

    childPid = fork();
    if (childPid == 0) {
        child (fd1, fd2, key);
    } else {
        parent (fd1, fd2, key);
    }

    wait ((void *)0);

    return (1);
}

void  parent (int fd1[2], int fd2[2], key_t key) {
    int stop, shmid;
    char *r;

    srand(getpid());

    close (fd1[0]); //close read end
    close (fd2[1]); //close write end

    /* Create the segment */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        cerr << "Error in shmid." << endl;
        exit (1);
    }

    /* Attach to the segment to get a pointer to it: */
    r = reinterpret_cast<char *> (shmat (shmid, NULL, 0));
    if (r == (char *)(-1)) {
        cerr << "Error in shmat." << endl;
        exit (1);
    }

    do {
        // Parent writes
        stop = generate (SHM_SIZE, r);
        cout << endl << ">>>>> PARENT writes   " << stop << " chars" << endl;
        write (fd1[1], &stop, sizeof (int));
        if (stop==0) {
            cout << endl << ">>>>> PARENT stops" << endl;
            break;
        }

        // Parent reads
        read (fd2[0], &stop, sizeof (int));
        cout << endl <<">>>>> PARENT receives " << stop << " char" << endl;
        if (stop!=0)
            convert_and_display  (r, stop);
    } while (stop != 0);

    /* Detach from the segment: */
    if (shmdt(r) == -1) {
        cerr << "Error in shmdt." << endl;
        exit (1);
    }

    close (fd1[1]);
    close (fd2[0]);

    return;
}

void child (int fd1[2], int fd2[2], key_t key) {
    int stop, shmid;
    char *r;

    srand(getpid());

    close (fd1[1]); //close write end
    close (fd2[0]); //close read end

    /* Create the segment */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        cerr << "Error in shmid." << endl;
        exit (1);
    }

    /* Attach to the segment to get a pointer to it: */
    r = reinterpret_cast<char *> (shmat (shmid, NULL, 0));
    if (r == (char *)(-1)) {
        cerr << "Error in shmat." << endl;
        exit (1);
    }

    do {
        // Child reads
        read (fd1[0], &stop, sizeof (stop));
        cout << endl << ">>>>> CHILD  receives " << stop << " char" << endl;
        if (stop!=0)
            convert_and_display (r, stop);
        else
            break;

        // Child writes
        stop = generate (SHM_SIZE, r);
        cout << endl << ">>>>> CHILD  writes   " << stop << " chars" << endl;
        write (fd2[1], &stop, sizeof (stop));
        if (stop==0) {
            cout << endl << ">>>>> CHILD stops" << endl;
        }
    } while (stop != 0);

    /* Detach from the segment: */
    if (shmdt(r) == -1) {
        cerr << "Error in shmdt." << endl;
        exit (1);
    }

    close (fd1[0]);
    close (fd2[1]);

    return;
}

/*
 *  Generate a text pointed by r with small letters, spaces, and newlines
 */

int generate (int max, char *r) {
    int i, n, val;

    n = rand () % max;
    for (i=0; i<n; i++) {
        val = rand ()%28;
        if (val==26)
            r[i] = (char) (' ');
        else
        if (val==27)
            r[i] = (char) ('\n');
        else
            r[i] = (char) ('a' + val);
        //fprintf (stdout, "%c", r[i]);
    }

    return n;
}

/*
 *  Convert the text pointed by r into capital letters, spaces, and newlines
 */

void convert_and_display  (char *r, int n) {
    int i;

    for (i=0; i<n; i++) {
        if (r[i]>='a' && r[i]<='z') {
            cout << (char) (r[i]+('A'-'a'));
        } else {
            cout << (char) r[i];
        }
    }

    return;
}
