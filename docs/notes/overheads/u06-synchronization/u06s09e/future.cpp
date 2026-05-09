#include <future>
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

// Thread function: C check if n is prime
bool is_prime (int num) {
  if (num <= 1) return false;
  if (num <= 3) return true;

  if (num % 2 == 0 || num % 3 == 0) return false;

  for (int i = 5; i <= std::sqrt(num); i += 6) {
      if (num % i == 0 || num % (i + 2) == 0) return false;
  }

  return true;
}

int main (int argc, char *argv[]) {
  std::future<bool> fut = std::async(
    std::launch::async, is_prime, atoi(argv[1]));
 
  // ... do other work ...

  bool ret = fut.get();
  if (ret)
    cout << "The number is prime." << endl;  
  else
    cout << "The number is NOT prime." << endl;      
    
  return 0; 
}
