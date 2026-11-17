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

template<typename IterType>
concept ForwardIter = std::forward_iterator<IterType>;

template<typename IterType>
concept BidirectionalIter = std::bidirectional_iterator<IterType>;

template<typename IterType>
concept InIter = std::input_iterator<IterType>;

template<typename IterType>
concept InOutIter = std::input_or_output_iterator<IterType>;

template<typename IterType, typename ValueType>
concept OutIter = std::output_iterator<IterType, ValueType>;

template<typename IterType>
concept RandomAccessIter = std::random_access_iterator<IterType>;

template<typename SentinelType, typename IterType>
concept SentinelFor = std::sentinel_for<SentinelType, IterType>;

using std::advance;
using std::distance;
using std::make_reverse_iterator;

} // namespace bt::types
