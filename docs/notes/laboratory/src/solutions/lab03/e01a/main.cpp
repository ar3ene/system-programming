#include "LIFO.h"
#include "FIFO.h"

#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {
  //
  // LIFO
  //

  LIFO<int> my_lifo_i;
  cout << "Stack of int." << endl;
  my_lifo_i.push (23);
  my_lifo_i.push (13);
  my_lifo_i.push (3);
  while (!my_lifo_i.empty()) {
    cout << "Pop: " << my_lifo_i.pop() << "; current size: " << my_lifo_i.getSize() << endl;
  }


  LIFO<string> my_lifo_s;
  cout << "Stack of string." << endl;  
  my_lifo_s.push ("my");
  my_lifo_s.push ("name");
  my_lifo_s.push ("is");
  my_lifo_s.push ("stefano");  
  while (!my_lifo_s.empty()) {
    cout << "Pop: " << my_lifo_s.pop() << "; current size: " << my_lifo_s.getSize() << endl;
  }

  //
  // FIFO
  //

  FIFO<int> my_fifo_i;
  cout << "Queue of int." << endl;
  my_fifo_i.enqueue (23);
  my_fifo_i.enqueue (13);
  my_fifo_i.enqueue (3);
  while (!my_fifo_i.empty()) {
    cout << "Pop: " << my_fifo_i.dequeue() << "; current size: " << my_fifo_i.getSize() << endl;
  }


  FIFO<string> my_fifo_s;
  cout << "Queue of string." << endl;  
  my_fifo_s.enqueue ("my");
  my_fifo_s.enqueue ("name");
  my_fifo_s.enqueue ("is");
  my_fifo_s.enqueue ("stefano");  
  while (!my_fifo_s.empty()) {
    cout << "Pop: " << my_fifo_s.dequeue() << "; current size: " << my_fifo_s.getSize() << endl;
  }
  
 #if 0
  //
  // FIFO
  //
  
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
      cout << "extracting element " << my_fifo.dequeue() << "; current size is " << my_fifo.getSize() << endl;
  }
#endif
  
  return 0;
}
