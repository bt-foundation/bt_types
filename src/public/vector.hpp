#pragma once

#include <array>
#include <vector>

#include "memory.hpp"

namespace bt::types {

template<typename T>
using Vec = std::pmr::vector<T>;

template<typename T, size_t Size>
using ConstVec = std::array<T, Size>;

using std::get;

} // namespace bt::types