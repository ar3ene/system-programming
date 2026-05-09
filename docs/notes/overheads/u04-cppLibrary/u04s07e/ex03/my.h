//
// Created by quer on 06/05/23.
//

#ifndef UNTITLED7_MY_H
#define UNTITLED7_MY_H

#include <iostream>
#include <vector>

// Declaration
template <class T>
class my_vector {
public:
    void my_push(const T &elem);
    T my_pop();

private:
    std::vector<T> v;
};

#endif //UNTITLED7_MY_H
