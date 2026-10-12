#pragma once

#include <vector>

#include "memory.hpp"

namespace bt::types {
template<typename T, typename AllocT = allocator<T>>
using vector = std::vector<T, AllocT>;
}