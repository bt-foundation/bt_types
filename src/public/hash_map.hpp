#pragma once

#include <unordered_map>

#include "memory.hpp"

namespace bt::types {
template<
    class KeyT,
    class ValueT,
    class HasherT = std::hash<KeyT>,
    class KeyeqT = std::equal_to<KeyT>,
    class AllocT = allocator<std::pair<const KeyT, ValueT>>>
using hash_map = std::unordered_map<KeyT, ValueT, HasherT, KeyeqT, AllocT>;
}