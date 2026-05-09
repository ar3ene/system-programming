//
// Created by michelangelo grosso on 3/19/2023.
//

#include <iostream>
#include <list>
#include "FIFO.h"

using namespace std;

template <class T>
void FIFO<T>::enqueue(const T &elem) {
    queue.push_back(elem);
}

template <class T>
T FIFO<T>::dequeue() {
    if(empty()) throw range_error("Queue is empty");

    T temp = queue.front();
    queue.pop_front();
    return temp;
}

template <class T>
void FIFO<T>::visit() const {
    for (auto &i : queue)
        cout << i << endl;
}

template <class T>
size_t FIFO<T>::getSize() const {
    return queue.size();
}

template <class T>
bool FIFO<T>::empty() const {
    return queue.empty();
}

int main() {
    FIFO<string> my_fifo;

    cout << "my_fifo " << (my_fifo.empty() ? "is " : "is not ") << "empty" << endl;

    cout << "Inserting two strings  into my_lifo..." << endl;

    my_fifo.enqueue("Better one egg today");
    my_fifo.enqueue("Than a chicken tomorrow");

    cout << "my_fifo " << (my_fifo.empty() ? "is " : "is not ") << "empty" << endl;

    cout << "my_fifo has " << my_fifo.getSize() << " elements" << endl;

    cout << "my_fifo contents:" << endl;

    my_fifo.visit();

    for (int i=0; i<3; i++) {
        try {
            cout << "extracting element " << my_fifo.dequeue() << "; current size is " << my_fifo.getSize() << endl;
        }
        catch (range_error &e) {
            cout << "Exception caught -- " << e.what() << endl;
        }
    }

    return 0;
}
