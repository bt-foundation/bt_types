#pragma once

#include <string>

#include "memory.hpp"

namespace bt::types {

template<typename ElemT = char, typename Traits = std::char_traits<ElemT>>
using String = std::pmr::basic_string<ElemT, Traits>;

}