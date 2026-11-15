#pragma once

#include <memory>
#include <memory_resource>

namespace bt::types {

using std::addressof;
using std::allocate_shared;

using AllocArgType = std::allocator_arg_t;
inline constexpr AllocArgType AllocArg {};

template<typename Alloc>
using AllocTraits = std::allocator_traits<Alloc>;

template<typename T>
using DefaultDelete = std::default_delete<T>;

using std::destroy;

template<typename T>
using EnableSharedFromThis = std::enable_shared_from_this<T>;

using std::make_shared;
using std::make_unique;

template<typename T>
using PtrTraits = std::pointer_traits<T*>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

using std::static_pointer_cast;
using std::uninitialized_fill;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

using std::uses_allocator_v;

template<typename T>
using Allocator = std::pmr::polymorphic_allocator<T>;

} // namespace bt::types