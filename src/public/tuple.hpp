#pragma once

#include <tuple>

#include "cstddef.hpp"

namespace bt::types {

using std::apply;
using std::forward_as_tuple;
using std::get;
using std::make_from_tuple;
using std::make_tuple;

template<typename... Ts>
using Tuple = std::tuple<Ts...>;

using std::tuple_cat;

template<size_t Index, typename Tuple>
using TupleElem = std::tuple_element<Index, Tuple>;

template<size_t Index, typename Tuple>
using TupleElemType = std::tuple_element_t<Index, Tuple>;

template<typename Tuple>
using TupleSize = std::tuple_size<Tuple>;

template<typename T>
constexpr size_t TupleSizeValue = std::tuple_size_v<T>;

} // namespace bt::types