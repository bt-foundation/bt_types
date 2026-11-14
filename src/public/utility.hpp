#pragma once

#include <utility>

namespace bt::types {

using std::as_const;
using std::declval;
using std::exchange;
using std::forward;
using std::in_place;
using std::in_place_t;
using std::in_place_type;
using std::in_place_type_t;
using std::index_sequence;
using std::index_sequence_for;
using std::make_index_sequence;
using std::make_pair;
using std::move;

template<typename T1, typename T2>
using Pair = std::pair<T1, T2>;

using std::piecewise_construct;
using std::piecewise_construct_t;
using std::swap;

} // namespace bt::types