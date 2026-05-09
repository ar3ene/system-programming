#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define LENGTH 30

int main(int argc, char *argv[]) {

FILE *f1, *f3;
int f2;
int res;
struct my_s_type {
  int id;
  int matr;
  char name[LENGTH+1];
  char surname[LENGTH+1];
  int mark;
} my_s;
int i;

if (argc != 4) {
printf ("Syntax error");
return -1;
}

if ((f1 = fopen (argv[1], "r")) == NULL) {
printf ("Error opening %s\n", argv[1]);
return -2;
}

if ((f2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {
printf ("Error opening %s in write mode\n", argv[2]);
return -3;
} 


do {
  res = fscanf(f1, "%d %d %s %s %d", &my_s.id, &my_s.matr, my_s.name, my_s.surname, &my_s.mark);
  if (res!=EOF) write(f2, &my_s, sizeof(struct my_s_type));
} while (res!= EOF);
fclose(f1);
close(f2);

if ((f2 = open(argv[2], O_RDONLY)) == -1) {
printf("Error opening %s in read mode\n", argv[2]);
return -3;
}
if ((f3 = fopen(argv[3], "w")) == NULL) {
printf("Error opening %s\n", argv[3]);
return -4;
}

do {
  res = read(f2, &my_s, sizeof(struct my_s_type));
  if (res) fprintf(f3, "%d %d %s %s %d\n", my_s.id, my_s.matr, my_s.name, my_s.surname, my_s.mark);
} while (res);

close(f2);
fclose(f3);
printf("Done!\n");
return 0;
}
