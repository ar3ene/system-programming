#include <functional>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

class C {
private:
    int i;
public:
    C () {
        cout << "[C]";
    }
    ~C() {
        cout << "[D]";
    }
    C (const C &n) {
        cout << "[CC]";
    }
    C &operator=(const C &n) {
        cout << "[CAO]";
        return *this;
    }
    C (C&& n) noexcept {
        cout << "[MC]";
    }
    C &operator=(C&&n) noexcept {
        cout << "[MAO]";
        return *this;
    }
    void set(int n)  {
        i = n;
    };
    int get () {
        return i;
    }
};

void swap (C &e1, C &e2) {
  C tmp;
  tmp=e1;
  e1=e2;
  e2=tmp;
  return;
}

int main() {
  cout << endl << "{01}"; C e1;
  cout << endl << "{02}"; C e2[5];
  cout << endl << "{03}"; C e3 = *new (std::nothrow) C;
  cout << endl << "{04}"; C *e4 = new C;
  cout << endl << "{05}"; C *e5 = new C[10];  
  
  cout << endl << "{06}"; C v1 = e1;
  cout << endl << "{07}"; C v2 = (std::move(e1));
  cout << endl << "{08}"; C v3;
  cout << endl << "{09}"; v3 = (std::move(e1));

  cout << endl << "{10}"; swap (e1, v3);

  cout << endl << "{11}"; return 0;
}
