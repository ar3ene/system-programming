/*
 *  StQ 24.03.2022
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define COPYINCR (1024*1024*1024) /* 1 GB */

int main(int argc, char *argv[]) {
    int fdin, fdout;
    void *src, *dst;
    size_t copysz;
    struct stat sbuf;
    off_t fsz = 0;

    if (argc != 3) {
        fprintf(stderr, "Run as: pgrm file_name_1 file_name_2.\n");
        exit(0);
    }
    if ((fdin = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "Error opening input file.\n");
        exit(0);
    }
    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 0) {
        fprintf(stderr, "Error opening output file.\n");
        exit(0);
    }
    /* need size of input file */
    if (fstat(fdin, &sbuf) < 0) {
        fprintf(stderr, "Error in fstat.\n");
        exit(0);
    }
    if (ftruncate(fdout, sbuf.st_size) < 0) {
        fprintf(stderr, "Error in ftruncate\n");
        exit(0);
    }
   while (fsz < sbuf.st_size) {
        if ((sbuf.st_size - fsz) > COPYINCR)
            copysz = COPYINCR;
        else
            copysz = sbuf.st_size - fsz;
        if ((src = mmap(0, copysz, PROT_READ, MAP_SHARED,
                        fdin, fsz)) == MAP_FAILED) {
            fprintf(stderr, "Error in input mmpap\n");
            exit(0);
        }
        if ((dst = mmap(0, copysz, PROT_READ | PROT_WRITE,
                        MAP_SHARED, fdout, fsz)) == MAP_FAILED) {
            fprintf(stderr, "Error in output mmap\n");
            exit(0);
        }
        memcpy(dst, src, copysz); /* does the file copy */
        munmap(src, copysz);
        munmap(dst, copysz);
        fsz += copysz;
    }
    exit(0);
}