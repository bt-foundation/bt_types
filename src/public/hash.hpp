#pragma once

#include <string_view>

namespace bt::types {

template<typename T>
using Hash = std::hash<T>;

}