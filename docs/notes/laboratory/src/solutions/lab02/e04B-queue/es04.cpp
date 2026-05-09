#include <iostream>
#include "FIFO.h"

using namespace std;

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
        catch (out_of_range &e) {
            cout << "Exception caught -- " << e.what() << endl;
        }
    }

    return 0;
}
