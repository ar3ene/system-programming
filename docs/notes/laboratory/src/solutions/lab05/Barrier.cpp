//
// Created by michelangelo grosso on 5/1/2023.
//

#include "Barrier.h"

Barrier::Barrier(size_t num_threads)
        : count_(num_threads), num_threads_(num_threads), generation_(0) {}

void Barrier::Wait() {
    std::unique_lock<std::mutex> lock(mutex_);
    std::size_t gen = generation_;
    if (--count_ == 0) {
        generation_++;
        count_ = num_threads_;
        cv_.notify_all();
    } else {
        cv_.wait(lock, [this, gen] { return gen != generation_; });
    }
}

void Barrier::wait_and_decrease() {
    std::unique_lock<std::mutex> lock(mutex_);
    std::size_t gen = generation_;
    num_threads_--;
    if (--count_ == 0) {
        generation_++;
        count_ = num_threads_;
        cv_.notify_all();
    } else {
        cv_.wait(lock, [this, gen] { return gen != generation_; });
    }
}