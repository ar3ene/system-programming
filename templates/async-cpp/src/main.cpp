#include "prime.hpp"

#include <future>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> values{1000003, 1000033, 1000037, 1000039};
    std::vector<std::future<bool>> futures;

    for (int n : values) {
        futures.push_back(std::async(std::launch::async, is_prime, n));
    }

    for (std::size_t i = 0; i < values.size(); ++i) {
        std::cout << values[i] << (futures[i].get() ? " is prime" : " is not prime") << '\n';
    }

    return 0;
}
