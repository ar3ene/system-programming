#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

// Shared memory segment size: 1K versus 24 bytes (to debug)
// #define SHM_SIZE 1024
#define SHM_SIZE 24

typedef struct mesg_buffer {
   long type;
   int num;
} msg_type;


void parent (int, key_t);
void child (int, key_t);
int generate (int, char *);
void convert_and_display (char *, int);

int main (int argc, char **argv) {
    key_t key;
    int msgid, childPid;

//    setbuf (stdout, 0);

    /* Make the key: The file must exist */
    if ((key = ftok("hello.txt", 3)) == -1) {
        cerr << "Error in ftok." << endl;
        exit (1);
    }

    msgid = msgget(key, 0666 | IPC_CREAT);    

    childPid = fork();
    if (childPid == 0) {
        child (msgid, key);
    } else {
        parent (msgid, key);
    }

    wait ((void *)0);

    msgctl(msgid, IPC_RMID, NULL);

    return (1);
}

void  parent (int mid, key_t key) {
    int stop, shmid;
    char *r;

    srand(getpid());

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

    msg_type msg {1, 0};

    do {
        // Parent writes
        stop = generate (SHM_SIZE, r);
        msg.num = stop;
        cout << endl << ">>>>> PARENT writes   " << stop << " chars" << endl;
        msgsnd(mid, &msg, sizeof(int), 0);
        if (stop==0) {
            cout << endl << ">>>>> PARENT stops" << endl;
            break;
        }

        // Parent reads
        msgrcv(mid, &msg, sizeof(int), 1, 0);
        stop = msg.num;
        cout << endl <<">>>>> PARENT receives " << stop << " char" << endl;
        if (stop!=0)
            convert_and_display  (r, stop);
    } while (stop != 0);

    /* Detach from the segment: */
    if (shmdt(r) == -1) {
        cerr << "Error in shmdt." << endl;
        exit (1);
    }

    return;
}

void child (int mid, key_t key) {
    int stop, shmid;
    char *r;

    srand(getpid());

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

    msg_type msg {1, 0};

    do {
        // Child reads
        msgrcv(mid, &msg, sizeof(int), 1, 0);
        stop = msg.num;
        cout << endl << ">>>>> CHILD  receives " << stop << " char" << endl;
        if (stop!=0)
            convert_and_display (r, stop);
        else
            break;

        // Child writes
        stop = generate (SHM_SIZE, r);
        cout << endl << ">>>>> CHILD  writes   " << stop << " chars" << endl;
        msg.num = stop;
        msgsnd(mid, &msg, sizeof(int), 0);
        if (stop==0) {
            cout << endl << ">>>>> CHILD stops" << endl;
        }
    } while (stop != 0);

    /* Detach from the segment: */
    if (shmdt(r) == -1) {
        cerr << "Error in shmdt." << endl;
        exit (1);
    }

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
