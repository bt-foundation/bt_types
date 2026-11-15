#pragma once

#include <string_view>

#include "memory.hpp"

namespace bt::types {

template<typename ElemT = char, typename Traits = std::char_traits<ElemT>>
using StringView = std::basic_string_view<ElemT, Traits>;

}