#pragma once

#include <unordered_set>

#include "memory.hpp"
#include "functional.hpp"
#include "hash.hpp"

namespace bt::types {

template<
    class KeyT,
    class HasherT = Hash<KeyT>,
    class KeyeqT = equal_to<KeyT>>
using HashSet = std::pmr::unordered_set<KeyT, HasherT, KeyeqT>;

}