#ifndef LAB2_FIFO_H
#define LAB2_FIFO_H

#include <list>
#include <string>

class FIFO {
public:
    void enqueue(string &);
    string dequeue();
    void visit() const;
    std::size_t getSize() const;
    bool empty() const;
private:
    std::list<string> queue{};
};

#endif //LAB2_FIFO_H
