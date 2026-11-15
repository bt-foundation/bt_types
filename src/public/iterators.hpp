#pragma once

#include <iterator>

namespace bt::types {

using BidirectionalIterTag = std::bidirectional_iterator_tag;
using ForwardIterTag = std::forward_iterator_tag;
using InputIterTag = std::input_iterator_tag;
using RandomAccessIterTag = std::random_access_iterator_tag;

template<typename IterType>
using IterTraits = std::iterator_traits<IterType>;

template<typename IterType>
using ReverseIter = std::reverse_iterator<IterType>;

using std::advance;
using std::distance;
using std::make_reverse_iterator;

} // namespace bt::types
