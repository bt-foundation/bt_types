#pragma once

#include <vector>

#include "memory.hpp"

namespace bt::types {
template<typename T>
using vector = std::pmr::vector<T>;
}