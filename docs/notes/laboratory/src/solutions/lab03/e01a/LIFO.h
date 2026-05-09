#ifndef LIFO_H
#define LIFO_H

#include <iostream>
#include <vector>

using std::cout;
using std::endl;

template <class T>
class LIFO {
  public:

  void push(const T &elem) {
    stack.push_back(elem);
  }

  T pop() {
    if(empty()) {
      cout << "Empty stack." << endl;
    }

    T temp = stack.back();
    stack.pop_back();
    return temp;
  }

  void visit() const {
    for (auto &i: stack)
        cout << i << " ";
    cout << endl;
 }

  size_t getSize() const {
    return stack.size();
  }

  bool empty() const {
    return stack.empty();
  }

  private:
    std::vector<T> stack;

};


#endif
