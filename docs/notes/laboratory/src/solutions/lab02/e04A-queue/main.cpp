#include "QueueClass.h"

int main() {
    QueueClass myQueue;
    int n;
    string val;

    myQueue.enqueue("first");
    myQueue.enqueue("second");
    myQueue.enqueue("third");
    myQueue.enqueue("four");

    myQueue.visit();
    std::cout << "Queue Size: " << myQueue.getSize() << endl;

    n = 0;
    while (!myQueue.empty()) {
        val = myQueue.dequeue();
        std::cout << "Dequeue " << n++ << ": " << val << std::endl;
    }

    return 0;
}
