//
// Created by quer on 06/05/23.
//

#ifndef UNTITLED7_MY_H
#define UNTITLED7_MY_H

#include <iostream>
#include <vector>

// Definition
template <class T>
void my_vector<T>::my_push(const T &elem) {
    v.push_back(elem);
}

template <class T>
T my_vector<T>::my_pop() {
    T tmp = v.back();
    v.pop_back();
    return tmp;
}

#endif //UNTITLED7_MY_H
