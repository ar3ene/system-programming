//
// Created by michelangelo grosso on 5/1/2023.
//

#ifndef LAB04_BARRIER_H
#define LAB04_BARRIER_H

#include <mutex>
#include <condition_variable>


class Barrier {
public:
    explicit Barrier(size_t num_threads);
    void Wait();
    void wait_and_decrease();

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    size_t count_;
    size_t num_threads_;
    size_t generation_;
};


#endif //LAB04_BARRIER_H
