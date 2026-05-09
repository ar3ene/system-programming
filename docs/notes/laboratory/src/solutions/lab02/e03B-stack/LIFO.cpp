#include <iostream>
#include "LIFO.h"

using namespace std;


void LIFO::push(const int &elem) {
    stack.push_back(elem);
}

int LIFO::pop() {
    if(empty()) throw out_of_range("Stack is empty");

    int temp = stack.back();
    stack.pop_back();
    return temp;
}

void LIFO::visit() const{
    for (auto &i: stack)
        cout << i << " ";
    cout << endl;
}

size_t LIFO::getSize() const {
    return stack.size();
}

bool LIFO::empty() const {
    return stack.empty();
}
