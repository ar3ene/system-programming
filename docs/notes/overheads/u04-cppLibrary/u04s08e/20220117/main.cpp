#include <functional>
#include <iostream>

int main() {
   using namespace std;
   int i = 3;
   int j = 5;
   function<int (void)> f = [&i, j] { return i + j; };
   i = 22;
   j = 44;
   cout << f() << endl;
}
