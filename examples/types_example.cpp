#include <print>

#include "types.hpp"

int main() {
    bt::types::vector<int> vec {1, 2, 3, 4, 5};

    for (auto& v : vec) {
        std::print("{}", v);
    }
    return 0;
}