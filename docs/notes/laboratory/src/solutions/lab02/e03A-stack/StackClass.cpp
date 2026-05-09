#include "StackClass.h"

void StackClass::push(int e) {
    stack.push_back(e);
    return;
}

int StackClass::pop() {
    int e;
    e = stack.back();
    stack.pop_back();
    return (e);
}

void StackClass::visit() {
    int n;
    cout << "Stack visit: " << endl;
    n = 0;
    for (auto e : stack)  {
        cout << n++ << ". " << e << endl;
    }
    return;
}

int StackClass::getSize() {
    return stack.size();
}

bool StackClass::empty() {
    return (stack.size()==0);
}
