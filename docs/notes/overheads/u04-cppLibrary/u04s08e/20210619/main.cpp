#include <iostream>

using std::cout;
using std::endl;

class Y {
private:
    int i;
public:
    Y () {
        cout << "[C]";
    }
    ~Y() {
        cout << "[D]";
    }
    Y (const Y &n) {
        cout << "[CC]";
    }
    Y &operator=(const Y &n) {
        cout << "[CAO]";
        return *this;
    }
    Y (Y&& n) noexcept {
        cout << "[MC]";
    }
    Y &operator=(Y&&n) noexcept {
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

void f1(Y y) { cout << "{f1}"; y.set(5); }
void f2(Y &y) { cout << "{f2}"; int n = y.get(); }

int main() {
  cout << "{1}"; Y y1;
  cout << "{2}"; Y y2=y1;
  cout << "{3}"; f1(y1);
  cout << "{4}"; f1(std::move(y1));
  cout << "{5}"; return 0;
}