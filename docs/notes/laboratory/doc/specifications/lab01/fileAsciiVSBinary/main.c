#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    FILE *fp;
    int fd;
    char sv[] = "This is a string";
    int iv = 10;
    float fv = 15.55;

    fp = fopen ("my_file_1.txt", "w");
    fprintf (fp, "%s", sv);
    fclose (fp);
    fp = fopen ("my_file_2.txt", "w");
    fprintf (fp, "%d", iv);
    fclose (fp);
    fp = fopen ("my_file_3.txt", "w");
    fprintf (fp, "%f", fv);
    fclose (fp);

    fd = open ("my_file_1.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    write (fd, sv, strlen (sv));
    close (fd);
    fd = open ("my_file_2.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    write (fd, &iv, sizeof (int));
    close (fd);
    fd = open ("my_file_3.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    write (fd, &fv, sizeof (float));
    close (fd);

    return 0;
}
