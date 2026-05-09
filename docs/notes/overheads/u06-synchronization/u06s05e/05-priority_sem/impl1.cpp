#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <semaphore>

using std::cout;
using std::endl;
using std::vector;
using std::thread;

const int TIME = 3;

// C++20 semaphore are neither copyble nor movable
// Need to create a class; then get problem in inserting objects in sets

vector<int> priorities;
vector<std::unique_ptr<std::binary_semaphore>> sems;
std::mutex m;

static void worker (int i, int priority) {
  m.lock();
  cout << "Locking thread " << i << " with priority " << priority << endl;
  m.unlock();
  priorities[i] = priority;
  (*sems[i]).acquire();
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
    priorities.push_back(INT_MAX);  
    sems.push_back(std::make_unique<std::binary_semaphore>(0));
  }
  
  for (i=0; i<n; i++) {
    priority = rand() % 100;
    pool.emplace_back([i, priority] { worker (i, priority); });
  }

  std::this_thread::sleep_for(std::chrono::seconds(rand()%TIME));
  
  for (i=0; i<n; i++) {
    auto min = min_element (priorities.begin(), priorities.end());
    int index = min - priorities.begin();
    m.lock();
    cout << "     Unlocking thread " << index << " with priority " << priorities[index] << endl;
    m.unlock();
    priorities[index] = INT_MAX;
    (*sems[index]).release();
  }

  for (i=0; i<n; i++) {
    pool[i].join();
  }

  cout << "Main exits." << endl;
  return (1);
}

