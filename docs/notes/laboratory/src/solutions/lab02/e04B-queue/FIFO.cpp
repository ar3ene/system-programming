#include <iostream>
#include "FIFO.h"

using namespace std;

void FIFO::enqueue(const string &elem) {
    queue.push_back(elem);
}

string FIFO::dequeue() {
    if(empty()) throw out_of_range("Queue is empty");

    string temp = queue.front();
    queue.pop_front();
    return temp;
}

void FIFO::visit() const {
    for (auto &i : queue)
        cout << i << endl;
}

size_t FIFO::getSize() const {
    return queue.size();
}

bool FIFO::empty() const {
    return queue.empty();
}

