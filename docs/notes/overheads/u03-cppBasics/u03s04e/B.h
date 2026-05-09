//
// Created by Antonio Vetrò on 04/05/22.
//

#ifndef CPP_SLIDE_04_B_H
#define CPP_SLIDE_04_B_H

#include "A.h"

class B {
    int b;
    friend class C;
public:
    void foo(A& a) ;
    int get_b();
};


#endif //CPP_SLIDE_04_B_H
