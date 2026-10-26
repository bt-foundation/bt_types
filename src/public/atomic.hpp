#pragma once

#include <atomic>

namespace bt::types {

template<typename T>
using Atomic = std::atomic<T>;

using AtomicBool = std::atomic_bool;

using AtomicInt16 = std::atomic_int16_t;
using AtomicInt32 = std::atomic_int32_t;
using AtomicInt64 = std::atomic_int64_t;

using AtomicSize = std::atomic_size_t;

using MemOrd = std::memory_order;

} // namespace bt::types