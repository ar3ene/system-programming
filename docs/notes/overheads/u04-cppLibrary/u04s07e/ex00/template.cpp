#include <iostream>

using std::cout;
using std::endl;

template<typename T>
void swap1 (T a, T b) {
  T tmp;
  tmp = a; a = b; b = tmp;
  return;
}

template<typename T>
void swap2 (T *a, T *b) {
  T tmp;
  tmp = *a; *a = *b; *b = tmp;
  return;
}

template<typename T>
void swap3 (T &a, T &b) {
  T tmp;
  tmp = a; a = b; b = tmp;
  return;
}

int main () {
  {
  int a, b;

  cout << "Int" << endl;

  a = 1; b = 10;
  cout << "Swap1: " << a << "," << b;
  swap1(a,b);
  cout << "-> " << a << "," << b << endl;

  a = 1; b = 10;
  cout << "Swap2: " << a << "," << b;
  swap2(&a,&b);
  cout << "-> " << a << "," << b << endl;

  a = 1; b = 10;
  cout << "Swap3: " << a << "," << b;
  swap3(a,b);
  cout << "-> " << a << "," << b << endl;
  }
  
  {
  float a, b;

  cout << "Real" << endl;
  
  a = 1.5; b = 10.1;
  cout << "Swap1: " << a << "," << b;
  swap1(a,b);
  cout << "-> " << a << "," << b << endl;

  a = 1.5; b = 10.1;
  cout << "Swap2: " << a << "," << b;
  swap2(&a,&b);
  cout << "-> " << a << "," << b << endl;

  a = 1.5; b = 10.1;
  cout << "Swap3: " << a << "," << b;
  swap3(a,b);
  cout << "-> " << a << "," << b << endl;
  }
  
  return 1;
}


