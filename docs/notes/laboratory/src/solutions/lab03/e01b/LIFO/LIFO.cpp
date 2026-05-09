//
// Created by michelangelo grosso on 3/19/2023.
//

#include <iostream>
#include <vector>
#include "LIFO.h"

using namespace std;


template <class T>
void LIFO<T>::push(const T &elem) {
    stack.push_back(elem);
}

template <class T>
T LIFO<T>::pop() {
    if(empty()) throw range_error("Stack is empty");

    T temp = stack.back();
    stack.pop_back();
    return temp;
}

template <class T>
void LIFO<T>::visit() const{
    for (auto &i: stack)
        cout << i << " ";
    cout << endl;
}

template <class T>
size_t LIFO<T>::getSize() const {
    return stack.size();
}

template <class T>
bool LIFO<T>::empty() const {
    return stack.empty();
}

int main() {

    LIFO<int> my_lifo;

    cout << "my_lifo " << (my_lifo.empty() ? "is " : "is not ") << "empty" << endl;

    cout << "Inserting two int values into my_lifo..." << endl;

    my_lifo.push(42);
    my_lifo.push(18);

    cout << "my_lifo " << (my_lifo.empty() ? "is " : "is not ") << "empty" << endl;

    cout << "my_lifo has " << my_lifo.getSize() << " elements" << endl;

    cout << "my_lifo contents:" << endl;

    my_lifo.visit();

    for (int i=0; i<3; i++) {
        try {
            cout << "extracting element " << my_lifo.pop() << "; current size is " << my_lifo.getSize() << endl;
        }
        catch (range_error &e) {
            cout << "Exception caught -- " << e.what() << endl;
        }
    }

}

