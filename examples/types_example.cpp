#include <print>

#include "types.hpp"

auto main() -> int {
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    bt::types::Vec<int> vec {1, 2, 3, 4, 5};

    for (auto& val : vec) {
        std::print("{}", val);
    }
    return 0;
}