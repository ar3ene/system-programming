#include <functional>
#include <iostream>
#include <vector>

using namespace std;
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

void f1(C e) { cout << "{f1}";}
void f2(C &e) { cout << "{f2}";}

int main() {
  cout << "{1}"; C e1, *e2;
  cout << "{2}"; C e3 = *new C;
  cout << "{3}"; C *e4 = new C[10];  
  cout << "{4}"; e1 = e3;
  cout << "{5}"; e2 = e4;
  cout << "{6}"; e1 = (std::move(e3));
  cout << "{7}"; e2 = (std::move(e4));
  cout << "{8}"; return 0;
}
