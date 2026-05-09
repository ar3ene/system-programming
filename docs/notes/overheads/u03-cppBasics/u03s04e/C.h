//
// Created by Antonio Vetrò on 04/05/22.
//

#ifndef CPP_SLIDE_04_C_H
#define CPP_SLIDE_04_C_H

#include "B.h"

class C {
    int c;
    friend class B;
public:
    void foo(A& a);
    void foo(B& b);
    void foo(C& c);
};

#endif //CPP_SLIDE_04_C_H
