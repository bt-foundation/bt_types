#pragma once

#include <string>

#include "memory.hpp"

namespace bt::types {
template<
    typename ElemT = char,
    typename Traits = std::char_traits<ElemT>,
    typename AllocT = allocator<ElemT>>
using string = std::basic_string<ElemT, Traits, AllocT>;
}