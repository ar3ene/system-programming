#ifndef FIFO_H
#define FIFO_H

#include <iostream>
#include <list>

template <class T>
class FIFO {
  public:
    void enqueue(const T &elem) {
      queue.push_back(elem);
    }

    T dequeue() {
      if(empty()) {
        cout << "Empty queue." << endl;
      }

      T temp = queue.front();
      queue.pop_front();
      return temp;
    }

    void visit() const {
      for (auto &i : queue)
        cout << i << endl;
    }

    size_t getSize() const {
      return queue.size();
    }

    bool empty() const {
      return queue.empty();
    }

  private:
    std::list<T> queue;
};


#endif
