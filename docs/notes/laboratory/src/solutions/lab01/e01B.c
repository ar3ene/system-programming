/*
 *  StQ 01.03.2022
 *  File copy: ASCII 2 Binary
 *  Only UNIX C library functions
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define L (30+1)

typedef struct student_s {
    int id;
    long int rn;
    char n[L];
    char s[L];
    int mark;
} student_t;

void display_int (int, int);

int main (int argc, char **argv) {
  char c, b, nl;
    int i, n, field, fdR, fdW;
    student_t student_d;

    if (argc != 4) {
        fprintf(stderr, "Usage: cp file-binary file-text file3-binary\n");
        exit(1);
    }

    /*
     *  Part 1: Read ASCII file and Write Binary file
     */

    fdR = open(argv[1], O_RDONLY);
    fdW = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fdR == -1 || fdW == -1) {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }

    field = i = student_d.id = student_d.rn = student_d.mark = 0;
    while ((n = read(fdR, &c, sizeof(char))) == sizeof(char)) {
        if (c=='\n') {
            n = write(fdW, &student_d, sizeof(student_t));
            fprintf(stdout, "%d %ld %s %s %d\n", student_d.id, student_d.rn,
                    student_d.n, student_d.s, student_d.mark);
            if (n != sizeof(student_t)) {
                fprintf(stderr, "Write in output file error (%s).\n", argv[2]);
                exit(1);
            }
            field = i = student_d.id = student_d.rn = student_d.mark = 0;
            continue;
        }
        if (c==' ') {
            field++;
            i = 0;
            continue;
        }
        switch (field)  {
            case 0: student_d.id = student_d.id * 10 + ((int) c - (int) '0'); break;
            case 1: student_d.rn = student_d.rn * 10 + ((int) c - (int) '0'); break;
            case 2: student_d.n[i++] = c; student_d.n[i] = '\0'; break;
            case 3: student_d.s[i++] = c; student_d.s[i] = '\0'; break;
            case 4: student_d.mark= student_d.mark * 10 + ((int) c - (int) '0'); break;
            default: fprintf (stderr, "Error: Case value.\n"); exit (1); break;

        }
    }

    close(fdR);
    close(fdW);

    /*
     *  Part 2: Read Binary file and Write ASCII file
     */

    fdR = open(argv[2], O_RDONLY);
    fdW = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fdR == -1 || fdW == -1) {
        fprintf(stderr, "Error opening output file.\n");
        exit(1);
    }

    b = ' ';
    nl = '\n';
    while ((n = read(fdR, &student_d, sizeof(student_t))) == sizeof(student_d)) {
        display_int (fdW, student_d.id);
        write (fdW, &b, sizeof(char));
        display_int (fdW, student_d.rn);
        write (fdW, &b, sizeof(char));
        for (i=0; i<strlen(student_d.n); i++)
            write (fdW, &student_d.n[i], sizeof(char));
        write (fdW, &b, sizeof(char));
        for (i=0; i<strlen(student_d.s); i++)
            write (fdW, &student_d.s[i], sizeof(char));
        write (fdW, &b, sizeof(char));
        display_int (fdW, student_d.mark);
        write (fdW, &nl, sizeof(char));
    }

    close(fdR);
    close(fdW);

    return 0;
}

void display_int (int fdW, int n) {
    int i;
    char v[L];

    for (i=0; n!=0; i++) {
        v[i] = (char) '0' + (char) (n%10);
        n = n/10;
    }
    i--;
    while (i>=0) {
        write (fdW, &v[i], sizeof(char));
        i--;
    }

    return;
}

