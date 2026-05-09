#include <iostream>
#include <LIFO.h>


int main() {

    LIFO my_lifo;

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
        catch (out_of_range &e) {
            cout << "Exception caught -- " << e.what() << endl;
        }
    }

}

