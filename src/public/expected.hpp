#pragma once

#include <expected>

namespace bt::types {

template<typename T, typename Err>
using Expected = std::expected<T, Err>;

} // namespace bt::types