// Two B and one A

#include <iostream>
#include <semaphore>
#include <thread>
#include <unistd.h>

using std::cout;
using std::endl;

std::counting_semaphore sa{1}, sb{2}, me{1};
int n;

static void TA (int);
static void TB (int);
  
int main (int argc, char **argv) {
  int n1, n2;

  if (argc != 2) {
    fprintf (stderr, "Syntax: %s num_threads\n", argv[0]);
    return (1);
  }
  n1 = atoi(argv[1]);
  n2 = 2 * n1;
  n = 0;
  
  std::thread ta (TA, n1);
  std::thread tb (TB, n2);  
  
  ta.join();
  tb.join();

  return (0);
}

static void TA (int nc) {
  for (int i=0; i<nc; i++) {
    sleep (rand()%2);
    sa.acquire();
    me.acquire();
    cout << "A";
    n++;
    if (n>=3) {
      cout << endl;
      n = 0;
      sa.release();
      sb.release();
      sb.release();      
    }
    me.release();    
  }
  
  return;
}  

static void TB (int nc) {
  for (int i=0; i<nc; i++) {
    sleep (rand()%2);
    sb.acquire();
    me.acquire();
    cout << "B";
    n++;
    if (n>=3) {
      cout << endl;
      n = 0;
      sa.release();
      sb.release();
      sb.release();      
    }
    me.release();    
  }
  
  return;
}  
	

