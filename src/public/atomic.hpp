#pragma once

#include <atomic>

namespace bt::types {
template<typename T>
using atomic = std::atomic<T>;

using atomic_bool = std::atomic_bool;

using atomic_i16 = std::atomic_int16_t;
using atomic_i32 = std::atomic_int32_t;
using atomic_i64 = std::atomic_int64_t;

using atomic_size_t = std::atomic_size_t;

using memory_order = std::memory_order;
} // namespace bt::types