#include <iostream>

#include "types.hpp"

auto main() -> int {
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    bt::types::Vec<int> vec {1, 2, 3, 4, 5};

    for (auto& val : vec) {
        std::cout << val;
    }
    return 0;
}