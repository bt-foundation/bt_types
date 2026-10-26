#pragma once

#include <vector>

#include "memory.hpp"

namespace bt::types {

template<typename T>
using Vector = std::pmr::vector<T>;

}