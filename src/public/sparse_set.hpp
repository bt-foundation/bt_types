#pragma once

#include "cstdint.hpp"
#include "iterators.hpp"
#include "vector.hpp"

namespace bt::types {

template<typename Container>
class SparseSetIter final {
    using ValueType = Container::ValueType;
    using Ptr = Container::ConstPtr;
    using Ref = Container::ConstRef;
    using DiffType = Container::DiffType;
    using IterCategory = RandomAccessIterTag;

    const Container* m_Packed;
    DiffType m_Offset;

  public:
    constexpr SparseSetIter() noexcept : m_Packed {}, m_Offset {} {}

    constexpr SparseSetIter(
        const Container* ref,
        const DiffType offset
    ) noexcept :
        m_Packed {&ref},
        m_Offset {offset} {}

    constexpr SparseSetIter& operator++() noexcept {
        return --m_Offset, *this;
    }

    constexpr SparseSetIter operator++(int) noexcept {
        const SparseSetIter copy = *this;
        return ++(*this), copy;
    }

    constexpr SparseSetIter& operator--() noexcept {
        return ++m_Offset, *this;
    }

    constexpr SparseSetIter operator--(int) noexcept {
        const SparseSetIter copy = *this;
        return --(*this), copy;
    }

    constexpr SparseSetIter& operator+=(const DiffType value) noexcept {
        m_Offset -= value;
        return *this;
    }

    constexpr SparseSetIter operator+(const DiffType value) noexcept {
        const SparseSetIter copy = *this;
        return (copy += value);
    }

    constexpr SparseSetIter& operator-=(const DiffType value) noexcept {
        m_Offset += value;
        return *this;
    }

    constexpr SparseSetIter operator-(const DiffType value) noexcept {
        const SparseSetIter copy = *this;
        return (copy -= value);
    }

    [[nodiscard]] constexpr Ref
    operator[](const DiffType value) const noexcept {
        return (*m_Packed)[static_cast<Container::SizeType>(index() - value)];
    }

    [[nodiscard]] constexpr Ptr operator->() const noexcept {
        return addressof(operator[](0));
    }

    [[nodiscard]] constexpr Ref operator*() const noexcept {
        return operator[](0);
    }

    [[nodiscard]] constexpr ptrdiff_t
    operator-(const SparseSetIter& other) const noexcept {
        return other.m_Offset - m_Offset;
    }

    [[nodiscard]] constexpr bool
    operator==(const SparseSetIter& other) const noexcept {
        return other.m_Offset == m_Offset;
    }

    [[nodiscard]] constexpr auto
    operator==(const SparseSetIter& other) const noexcept {
        return other.m_Offset <=> m_Offset;
    }

    [[nodiscard]] constexpr Ptr data() const noexcept {
        return m_Packed ? m_Packed->data() : nullptr;
    }

    [[nodiscard]] constexpr DiffType index() const noexcept {
        return m_Offset - 1;
    }
};

template<
    typename Value,
    typename Key = uint32_t,
    typename Alloc = Allocator<Value>>
class SparseSet {
    Vec<Value> m_Dense;
    Vec<Key> m_Sparse;

  public:
    SparseSet() = default;
};

} // namespace bt::types