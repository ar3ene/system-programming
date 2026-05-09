#include <iostream>
#include <vector>

using std::cout;
using std::endl;

class A
{
   public:
    ~A() { std::cout << "*"; }
    A() { std::cout<<"a"; }
    A(const A&) { std::cout<<"&"; }
};

int main()
{
    int i=3;
    std::vector<A> var(i);
}

