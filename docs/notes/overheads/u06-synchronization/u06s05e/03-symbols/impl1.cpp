// +++++++ N
// ------- N
// ******* N

#include <iostream>
#include <semaphore>
#include <thread>
#include <unistd.h>

using std::cout;
using std::endl;

std::counting_semaphore sp{1}, sm{0}, ss{0}, snl{0};

static void TP (int);
static void TM (int);
static void TS (int);
static void TNL ();
  
int main (int argc, char **argv) {
  int n;

  if (argc != 2) {
    fprintf (stderr, "Syntax: %s num_threads\n", argv[0]);
    return (1);
  }
  n = atoi(argv[1]);
  
  std::thread tp (TP, n);
  std::thread tm (TM, n);
  std::thread ts (TS, n);
  std::thread tnl (TNL);      

  // Threads never stop-join (but I cannot return or I will stop all threads)
  tp.join();
  tm.join();
  ts.join();
  tnl.join();  

  return (0);
}

static void TP (int n) {
  int np = 0;
  
  while (1) {
    sp.acquire();
    cout << "+";
    np++;
    if (np<n) {
      sp.release();
    } else {
      np = 0;
      snl.release();
    }
  }
  
  return;
}  

static void TM (int n) {
  int nm = 0;
  
  while (1) {
    sm.acquire();
    cout << "-";
    nm++;
    if (nm<n) {
      sm.release();
    } else {
      nm = 0;
      snl.release();
    }
  }
  
  return;
}  

static void TS (int n) {
  int ns = 0;
  
  while (1) {
    ss.acquire();
    cout << "*";
    ns++;
    if (ns<n) {
      ss.release();
    } else {
      ns = 0;
      snl.release();
    }
  }
  
  return;
}  

static void TNL () {
  int nnl = 0;
  
  while (1) {
    snl.acquire();
    nnl++;
    cout << endl;
    sleep (rand()%2);
    if (nnl==1) {
      sm.release();
    } else {
      if (nnl==2) {
        ss.release();
      } else {
        sp.release();
	nnl = 0;
      }
    }
  }
  
  return;
}  
