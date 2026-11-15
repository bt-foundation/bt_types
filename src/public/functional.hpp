#pragma once

#include <functional>

namespace bt::types {

template<typename FuncType>
using Function = std::function<FuncType>;

template<typename KeyType>
using Hash = std::hash<KeyType>;

using std::invoke;

using std::equal_to;
using std::less;

} // namespace bt::types
