#pragma once

#include <memory>
#include <memory_resource>

namespace bt::types {
template<typename T>
using allocator = std::pmr::polymorphic_allocator<T>;
}