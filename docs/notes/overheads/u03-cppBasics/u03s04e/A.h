//
// Created by Antonio Vetrò on 04/05/22.
//

#ifndef CPP_SLIDE_04_A_H
#define CPP_SLIDE_04_A_H

class A {
    int a;
    friend class B;
public:
    friend void foo(A& other) ;
    int get_a();
};

#endif //CPP_SLIDE_04_A_H
