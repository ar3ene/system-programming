#include <iostream>
#include <vector>

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

void f1(Y y) { cout << "{f1}";}
void f2(Y &y) { cout << "{f2}";}

int main() {
  cout << "{1}"; Y y1;
  cout << "{2}"; f1(y1);
  cout << "{3}"; f2(y1);
  cout << "{4}"; Y *y2 = new Y;
  cout << "{5}"; Y y3;
  cout << "{6}"; y3 = (std::move(y1));
  cout << "{7}"; return 0;
}

