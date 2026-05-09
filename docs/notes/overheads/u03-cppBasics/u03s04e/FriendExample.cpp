//
// Created by Antonio Vetrò on 04/05/22.
//

#include "A.h"
#include "B.h"
#include "C.h"
#include <iostream>

using namespace std;

int main(){

    A a1 ;
    B b1 ;
    C c1 ;

    b1.foo(a1);
    c1.foo(b1);
    c1.foo(a1);


    cout << a1.get_a() << endl;
    cout << b1.get_b() << endl;

}