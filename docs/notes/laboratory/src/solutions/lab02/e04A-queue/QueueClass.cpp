#include "QueueClass.h"

void QueueClass::enqueue(string e) {
    queue.push_front(e);
    return;
}

string QueueClass::dequeue() {
    string e;
    e = queue.back();
    queue.pop_back();
    return (e);
}

void QueueClass::visit() {
    int n;
    cout << "Queue visit: " << endl;
    n = 0;
    for (auto e : queue)  {
        cout << n++ << ". " << e << endl;
    }
    return;
}

int QueueClass::getSize() {
    return queue.size();
}

bool QueueClass::empty() {
    return (queue.size()==0);
}
