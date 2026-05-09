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

void f1(Y y) { cout << "{f1}"; }
Y f2(Y &y) { cout << "{f2}"; Y ay; return ay; }
void f3(Y y1, Y &y2){ cout << "{f3}"; }

int main() {
    cout << "{1}"; Y y1, y2, y3;
    cout << "{2}"; y1=y2;
    cout << "{3}"; f3(y1, y3);
    cout << "{4}"; Y y4 = f2(y1);
    cout << "{5}"; return 0;
}