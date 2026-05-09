#include <iostream>
#include <set>
#include <algorithm>

int main() {
  std::set<int> a = {1, 2, 3, 4, 5};
  std::set<int> b = {3, 4, 5, 6, 7};
  std::set<int> in, dif;

  std::set_intersection(a.begin(), a.end(),
                        b.begin(), b.end(),
                        std::inserter(in, in.begin()));

  std::cout << "Intersection: ";
  for (int num : in) {
    std::cout << num << " ";
  }
  std::cout << std::endl;

  std::set_difference(a.begin(), a.end(),
                      b.begin(), b.end(),
                      std::inserter(dif, dif.begin()));

  std::cout << "Difference: ";
  for (int num : dif)
    std::cout << num << " ";
  std::cout << std::endl; 

  return 0;
}
