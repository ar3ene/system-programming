#ifndef _STACKCLASS
#define _STACKCLASS

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

class StackClass {
public:
    void push(int);
    int pop();
    void visit();
    int getSize();
    bool empty();
private:
    vector<int> stack;
};

#endif
