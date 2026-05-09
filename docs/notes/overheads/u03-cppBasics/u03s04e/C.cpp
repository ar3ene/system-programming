//
// Created by Antonio Vetrò on 04/05/22.
//

#include "C.h"

void C::foo(A& a){/*a.a = 42;*/}; //compilation error, C is not friend of A
void C::foo(B &b) {b.b =42;} ;