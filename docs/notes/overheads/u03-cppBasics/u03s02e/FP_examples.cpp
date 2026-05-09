//
// Created by Antonio Vetro' on 09/04/22.
//
#include "iostream"
#include "MyClass.h"

using namespace std;

/* passage by value */
    void fv(int j) {
        j=27;
    }

/* passage by address (pointer) */
    void fa(int *p){
    if (p!=nullptr)
        *p =27;
    }

/* passage by reference */
    void fr(int &r){
        r= 27;
    }

int main(int argc, char ** argv){
    int i=10;

    /* passage by value */
    fv(i);
    cout << "Value  of i: " << i << endl;

    /* passage by address (pointer) */
    fa(&i);
    cout << "Value  of i: " << i << endl;

    /* passage by reference */
    i=10;
    fr(i);
    cout << "Value  of i: " << i << endl;

    /* new and delete */

    int * var = new int;
    int * var_init = new int(12);
    int * vect1 = new int[10]; //cannot be nullptr
    int * vect2 = new (std::nothrow) int[20]; //cannot be nullptr
    MyClass *mc= new MyClass;


    delete var;
    delete var_init;
    delete[] vect1;
    delete[] vect2;
    delete mc;

    return 0;
}
