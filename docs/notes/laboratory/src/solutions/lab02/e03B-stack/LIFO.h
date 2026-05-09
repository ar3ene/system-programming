#ifndef LAB2_LIFO_H
#define LAB2_LIFO_H

#include <vector>

class LIFO {
public:
    void push(const int &);
    int pop();
    void visit() const;
    std::size_t getSize() const;
    bool empty() const;
private:
    std::vector<int> stack{};
};


#endif //LAB2_LIFO_H
