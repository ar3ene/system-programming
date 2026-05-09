#include "math_utils.hpp"

#include <stdexcept>

int dot_product(const std::vector<int> &a, const std::vector<int> &b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("dot_product: size mismatch");
    }

    int sum = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}
