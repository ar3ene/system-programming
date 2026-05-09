//
// Created by quer on 06/05/23.
//

#ifndef UNTITLED7_MY_H
#define UNTITLED7_MY_H

#include <iostream>
#include <vector>

template <class T>
class my_vector {
public:
    void my_push(const T &elem) {
        v.push_back(elem);
    }

    T my_pop() {
        T tmp = v.back();
        v.pop_back();
        return tmp;
    }

private:
    std::vector<T> v;
};

#endif //UNTITLED7_MY_H
