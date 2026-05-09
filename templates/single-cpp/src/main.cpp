#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
    std::vector<int> v{4, 1, 6, 2, 3};
    std::sort(v.begin(), v.end());

    std::cout << "single-cpp template\n";
    std::cout << "argc = " << argc << '\n';
    for (int x : v) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << '\n';
    }
    return 0;
}
