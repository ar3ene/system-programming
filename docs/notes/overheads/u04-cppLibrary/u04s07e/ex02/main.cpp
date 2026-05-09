#include "my.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {
  my_vector<int> v;
  cout << "[Push]1"; v.my_push (1);
  cout << "[Push]2"; v.my_push (2);
  cout << "[Push]3"; v.my_push (3);
  cout << "[Pop]" << v.my_pop ();
  cout << "[Pop]" << v.my_pop ();
  cout << "[Pop]" << v.my_pop ();
  return 0;
}
