#include "math_utils.hpp"

#include <iostream>
#include <vector>

int main() {
    const std::vector<int> a{1, 2, 3};
    const std::vector<int> b{4, 5, 6};

    std::cout << "multifile-cpp template\n";
    std::cout << "dot_product = " << dot_product(a, b) << '\n';
    return 0;
}
