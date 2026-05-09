//
// Created by michelangelo grosso on 3/19/2023.
//

#ifndef LAB2_LIFO_H
#define LAB2_LIFO_H

template <class T>
class LIFO {
public:
    void push(const T &);
    T pop();
    void visit() const;
    std::size_t getSize() const;
    bool empty() const;
private:
    std::vector<T> stack;
};


#endif //LAB2_LIFO_H
