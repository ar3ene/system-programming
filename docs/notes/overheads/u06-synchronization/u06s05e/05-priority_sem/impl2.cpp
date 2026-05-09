#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <thread>
#include <semaphore>

using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::make_pair;
using std::thread;

const int TIME = 3;

// C++20 semaphore are neither copyble nor movable
// Need to create a class; then get problem in inserting objects in sets

map<int,std::unique_ptr<std::binary_semaphore>> my_sem;
std::mutex m;

static void worker (int i, int priority) {
  m.lock();
  cout << "Locking thread " << i << " with priority " << priority << endl;
  m.unlock();
  my_sem.insert ({priority,std::make_unique<std::binary_semaphore>(0)});
  (*my_sem[priority]).acquire();
  m.lock();
  cout << "          Unlocked thread " << i << " with priority " << priority << endl;  
  m.unlock();
  return;
}

int main (int argc, char *argv[]) {
  int i, priority;

  if (argc != 2) {
    cout << "Syntax: " << argv[0] << " num_threads\n";
    return (1);
  }
  int n = atoi (argv[1]);

  vector<thread> pool;  

  for (i=0; i<n; i++) {
    priority = (i+1) * 10;
    pool.emplace_back([i, priority] { worker (i, priority); });
  }

  std::this_thread::sleep_for(std::chrono::seconds(rand()%TIME));

  i = 0;
  for (const auto &t : my_sem) {
    m.lock();
    cout << "     Unlocking thread " << i++ << " with priority " << t.first << endl;
    m.unlock();
    (*(t.second)).release();
  }

  for (i=0; i<n; i++) {
    pool[i].join();
  }

  cout << "Main exits." << endl;
  return (1);
}

