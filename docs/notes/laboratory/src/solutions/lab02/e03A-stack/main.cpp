#include "StackClass.h"

int main() {
    StackClass myStack;
    int n, val;

    myStack.push(2);
    myStack.push(3);
    myStack.push(7);

    myStack.visit();
    std::cout << "Stack Size: " << myStack.getSize() << endl;

    n = 0;
    while (!myStack.empty()) {
        val = myStack.pop();
        std::cout << "Pop " << n++ << ": " << val << std::endl;
    }

    return 0;
}
