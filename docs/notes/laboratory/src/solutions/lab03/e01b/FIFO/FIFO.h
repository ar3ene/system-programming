//
// Created by michelangelo grosso on 3/19/2023.
//

#ifndef LAB2_FIFO_H
#define LAB2_FIFO_H

template <class T>
class FIFO {
public:
    void enqueue(const T &);
    T dequeue();
    void visit() const;
    std::size_t getSize() const;
    bool empty() const;
private:
    std::list<T> queue;
};


#endif //LAB2_FIFO_H
