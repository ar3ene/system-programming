#include <iostream>
#include <fstream>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define L 30+1

using namespace std;

typedef struct student_s {
  int id;
  long int rn;
  char n[L];
  char s[L];
  int mark;
} student_t;

void f_t1(void *, int);
void f_t2(void *, int);

int main(int argc, char *argv[]) {
cout << "original file: " << endl;
ifstream fa(argv[1], ios::binary);
student_t s;
while (fa.read(reinterpret_cast<char *>(&s), sizeof(student_t))) {
  cout << s.id << " " << s.rn << " " << s.n << " " << s.s << " " << s.mark << endl;
}
fa.close();


int fin = open(argv[1], O_RDWR);
if (fin == -1) {
  cerr << "Error opening file." << endl;
  exit(1);
}

struct stat fileInfo;
    if (fstat(fin, &fileInfo) == -1) {
        std::cerr << "Failed to get file size." << std::endl;
        close(fin);
        return 1;
    }
off_t fileSize = fileInfo.st_size;

void* src = mmap (0, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fin, 0);

if (src == MAP_FAILED) {
        std::cerr << "Failed to memory map the file." << std::endl;
        close(fin);
        return 1;
    }

thread t1(f_t1, src, fileSize);
thread t2(f_t2, src, fileSize);

t1.join();
t2.join();

munmap(src, fileSize);
close(fin);

fa.open(argv[1], ios::binary);
while (fa.read(reinterpret_cast<char *>(&s), sizeof(student_t))) {
  cout << s.id << " " << s.rn << " " << s.n << " " << s.s << " " << s.mark << endl;
}
fa.close();
return 0;

}

void f_t1(void *s, int n) {
for (int i=0; i<n/sizeof(student_t); ++i)
{
  reinterpret_cast<student_t *> (s)[i].rn++;
}
}

void f_t2(void *s, int n) {
for (int i= n/sizeof(student_t)-1; i>=0; --i)
{
  reinterpret_cast<student_t *> (s)[i].mark--;
}
}
