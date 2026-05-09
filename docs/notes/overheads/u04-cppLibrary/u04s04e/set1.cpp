#include <iostream>
#include <algorithm>
#include <iterator>
#include <set>

int main() {
  int a[] = { 1, 3, 5 };
  int b[] = { 0, 2, 4, 6 };
  std::set<int> s;

  std::set_union (std::begin(a), std::end(a),
                  std::begin(b), std::end(b),
                  std::inserter (s, s.begin()));

  for (int x : s)
    std::cout << x << ' ';                 
  std::cout << std::endl;

  return 0;
}
