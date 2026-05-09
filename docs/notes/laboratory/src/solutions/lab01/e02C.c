#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define LENGTH 30

int main(int argc, char *argv[]) {

int f1;
char read_in[100];
char cmd[2];
int num; 

struct my_s_type {
  int id;
  int matr;
  char name[LENGTH+1];
  char surname[LENGTH+1];
  int mark;
} my_s;
int i;

if (argc != 2) {
printf ("Syntax error");
return -1;
}

if ((f1 = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
printf("Error opening %s in read mode\n", argv[1]);
return -2;
}

do
{
printf("> ");
scanf("%s", cmd);
if (cmd[0] == 'R') {
  scanf("%d", &num);
    lseek(f1, (num-1)*sizeof(struct my_s_type), SEEK_CUR);
    read(f1, &my_s, sizeof(struct my_s_type));
    printf("Entry n. %d: %d %d %s %s %d\n", num, my_s.id, my_s.matr, my_s.name, my_s.surname, my_s.mark);
    lseek(f1, -(num)*sizeof(struct my_s_type), SEEK_CUR);
} else if (cmd[0] == 'W') {
  scanf("%d", &num);
  lseek(f1, (num-1)*sizeof(struct my_s_type), SEEK_CUR);
  printf(" > ");
  scanf("%d %d %s %s %d", &my_s.id, &my_s.matr, my_s.name, my_s.surname, &my_s.mark);
  write(f1, &my_s, sizeof(struct my_s_type));
  lseek(f1, -(num)*sizeof(struct my_s_type), SEEK_CUR);
}
}
while (cmd[0]!='E');

close(f1);
printf("Done!\n");
return 0;
}
