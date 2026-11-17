#pragma once

#include <string>

#include "memory.hpp"

namespace bt::types {

template<typename ElemT = char, typename Traits = std::char_traits<ElemT>>
using BasicString = std::pmr::basic_string<ElemT, Traits>;

using String = BasicString<>;

using std::to_string;

} // namespace bt::types