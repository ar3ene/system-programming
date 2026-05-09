#include "prime.hpp"

bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}
