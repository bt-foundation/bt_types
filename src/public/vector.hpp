#pragma once

#include <vector>

#include "memory.hpp"

namespace bt::types {

template<typename T>
using Vec = std::pmr::vector<T>;

}