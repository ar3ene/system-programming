#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
  
struct person{
  string name;
  
  person(string n):name(n){}  // Constructor
};

int main() {
  shared_ptr<person> p1 = make_shared<person>("Jack");
  shared_ptr<person> p2;
  shared_ptr<person> p3;

  cout << p1.use_count() << endl;  // 1  
  p2 = p1;
  cout << p2.use_count() << endl;  // 2
  weak_ptr<person> wp(p1);  
  cout << wp.use_count() << endl;  // 2

  if (p3 = wp.lock()) {
    cout << p3->name << endl;     //  Jack
  }
  cout << p1.use_count() << endl;   // 3
  cout << p3.use_count() << endl;   // 3  
  
  p1.reset(new person("rose"));
  cout << p1.use_count() << endl;   // 1
  cout << p2.use_count() << endl;   // 2
  cout << p3.use_count() << endl;   // 2  
  p2.reset();	
  cout << p2.use_count() << endl;   // 0
  cout << p3.use_count() << endl;   // 1
#if 1
  // If executed the pointer wp.expired is true
  p3.reset();
  cout << p3.use_count() << endl;   // 0   
#endif
 
  if (wp.expired()) {				
    cout << "Pointer KO !" << endl;
  }  else {
    // Impossibile to use wp->name
    // A weak pointer does not provide direct access to a resource
    // Code that needs to use a resource does so through a shared_ptr createad calling lock()
    cout << "Pointer OK: " << p3->name << endl;    
  }

  return (1);
}
