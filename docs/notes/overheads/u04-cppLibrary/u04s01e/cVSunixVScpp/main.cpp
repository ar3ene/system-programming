#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

using std::cout;
using std::endl;
using std::fstream;
using std::ofstream;
using std::ifstream;

void c(long int);
void c_nobuf(long int);
void cunix(long int);
void cunix_array(long int);
void cpp(long int);
void cpp_bin(long int);

int main(int argc, char *argv[]) {
   long int n = atoi (argv[1]);
   srand ((unsigned) getpid ());

   c (n);
   c_nobuf (n);
   cunix (n);
   cunix_array (n);   
   cpp (n);
   cpp_bin (n);
   
   return (1);
}

//
// C Section
//

void c(long int n)  {
   FILE *fp;
   int i, val;
   long long microseconds;
   
   cout << "C-Buffered Section" << endl;

   auto start = std::chrono::high_resolution_clock::now();
   fp = fopen ("c.txt", "w");
   for (i=0; i<n; i++) {
     val = rand();
     fprintf (fp, "%d\n", val);
   }
   fclose (fp);
   auto elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  C    Write Time: " << microseconds << endl;

   start = std::chrono::high_resolution_clock::now();   
   fp = fopen ("c.txt", "r");
   while (fscanf (fp, "%d", &val) != EOF) {
     //fprintf (stdout, "%d\n", val);
     i++;
   }
   fclose (fp);
   elapsed = std::chrono::high_resolution_clock::now() - start;   
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  C    Read  Time: " << microseconds << endl;

   return;
}

//
// C BUF=0 Section
//

void c_nobuf(long int n)  {
   FILE *fp;
   int i, val;
   long long microseconds;
   
   cout << "C-Unbuffered Section" << endl;

   auto start = std::chrono::high_resolution_clock::now();
   fp = fopen ("c.txt", "w");
   setbuf(fp,0);
   for (i=0; i<n; i++) {
     val = rand();
     fprintf (fp, "%d\n", val);
   }
   fclose (fp);
   auto elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  C    Write Time: " << microseconds << endl;

   start = std::chrono::high_resolution_clock::now();   
   fp = fopen ("c.txt", "r");
   while (fscanf (fp, "%d", &val) != EOF) {
     //fprintf (stdout, "%d\n", val);
     i++;
   }
   fclose (fp);
   elapsed = std::chrono::high_resolution_clock::now() - start;   
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  C    Read  Time: " << microseconds << endl;

   return;
}

//
// UNIX Section
//

void cunix (long int n) {
   int fd;
   int i, val;
   long long microseconds;
   
   cout << "UNIX Section" << endl;   

   auto start = std::chrono::high_resolution_clock::now();
   fd = open ("unix.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
   for (i=0; i<n; i++) {
     val = rand();
     write (fd, &val, sizeof (int));
   }
   close (fd);
   auto elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  UNIX Write Time: " << microseconds << endl;

   start = std::chrono::high_resolution_clock::now();
   fd = open ("unix.bin", O_RDONLY);
   while (read (fd, &val, sizeof (int)) != 0) {
     //fprintf (stdout, "%d\n", val);
     i++;
   }
   close (fd);
   elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  UNIX Read  Time: " << microseconds << endl;

   return;
}


//
// UNIX ARRAY Section
//

void cunix_array (long int n) {
   int fd;
   int i, *v;
   long long microseconds;

   v = (int *) malloc (n * sizeof (int));
   
   cout << "UNIX Array Section" << endl;   

   auto start = std::chrono::high_resolution_clock::now();
   fd = open ("unix.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
   for (i=0; i<n; i++) {
     v[i] = rand();
   }
   write (fd, v, n*sizeof (int));
   close (fd);
   auto elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  UNIX Write Time: " << microseconds << endl;

   start = std::chrono::high_resolution_clock::now();
   fd = open ("unix.bin", O_RDONLY);
   read (fd, v, n*sizeof (int));
   close (fd);
   elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  UNIX Read  Time: " << microseconds << endl;

   return;
}

//
// C++ Section
//

void cpp (long int n) {
   fstream s;
   int i, val;
   long long microseconds;
   
   cout << "C++ Section" << endl;      

   auto start = std::chrono::high_resolution_clock::now();
   s.open ("cpp.txt", ofstream::out);
   for (i=0; i<n; i++) {
     val = rand();
     s << val << endl;
   }
   s.close();
   auto elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  C++  Write Time: " << microseconds << endl;

   start = std::chrono::high_resolution_clock::now();
   s.open ("cpp.txt", ifstream::in);
   while (s >> val) {
     //cout << val << endl;
     i++;
   }
   s.close();
   elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  C++   Read Time: " << microseconds << endl;
   
   return;
}

//
// C++ Binary Section
//

void cpp_bin (long int n) {
   fstream s;
   int i, val;
   long long microseconds;
   
   cout << "C++ Binary Section" << endl;      

   auto start = std::chrono::high_resolution_clock::now();
   s.open ("cpp_bin.bin", ofstream::out | ofstream::binary);
   for (i=0; i<n; i++) {
     val = rand();
     s.write ((char *) &val, sizeof (int));
   }
   s.close();
   auto elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  C++  Write Time: " << microseconds << endl;

   start = std::chrono::high_resolution_clock::now();
   s.open ("cpp_bin.bin", ifstream::in | ifstream::binary);
   while (s.read((char *) &val, sizeof (int))) {
     //cout << val << endl;
     i++;
   }
   s.close();
   elapsed = std::chrono::high_resolution_clock::now() - start;
   microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();  
   cout << "  C++   Read Time: " << microseconds << endl;
   
   return;
}
