#pragma once

#include <unordered_map>

#include "memory.hpp"
#include "functional.hpp"
#include "hash.hpp"

namespace bt::types {

template<
    class KeyT,
    class ValueT,
    class HasherT = Hash<KeyT>,
    class KeyeqT = equal_to<KeyT>>
using HashMap = std::pmr::unordered_map<KeyT, ValueT, HasherT, KeyeqT>;

}