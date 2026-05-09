#include "my.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

// Definition
template <class T>
void my_vector<T>::my_push(const T &elem) {
    v.push_back(elem);
}

template <class T>
T my_vector<T>::my_pop() {
    T tmp = v.back();
    v.pop_back();
    return tmp;
}

int main() {
  my_vector<int> v;
  cout << "[Push]1"; v.my_push (10);
  cout << "[Push]2"; v.my_push (20);
  cout << "[Push]3"; v.my_push (30);
  cout << "[Pop]" << v.my_pop ();
  cout << "[Pop]" << v.my_pop ();
  cout << "[Pop]" << v.my_pop ();
  return 0;
}
