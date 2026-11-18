#pragma once

#include "cstdint.hpp"
#include "iterators.hpp"
#include "vector.hpp"

namespace bt::types {

template<typename Container>
class SparseSetIter final {
  public:
    using ValueType = Container::ValueType;
    using Ptr = Container::ConstPtr;
    using Ref = Container::ConstRef;
    using DiffType = Container::DiffType;
    using IterCategory = RandomAccessIterTag;

    constexpr SparseSetIter() noexcept : m_Packed {}, m_Offset {} {}

    constexpr SparseSetIter(
        const Container* ref,
        const DiffType offset
    ) noexcept :
        m_Packed {&ref},
        m_Offset {offset} {}

    constexpr auto operator++() noexcept -> SparseSetIter& {
        return --m_Offset, *this;
    }

    constexpr auto operator++(int) noexcept -> SparseSetIter {
        const SparseSetIter copy = *this;
        return ++(*this), copy;
    }

    constexpr auto operator--() noexcept -> SparseSetIter& {
        return ++m_Offset, *this;
    }

    constexpr auto operator--(int) noexcept -> SparseSetIter {
        const SparseSetIter copy = *this;
        return --(*this), copy;
    }

    constexpr auto operator+=(const DiffType value) noexcept -> SparseSetIter& {
        m_Offset -= value;
        return *this;
    }

    constexpr auto operator+(const DiffType value) noexcept -> SparseSetIter {
        const SparseSetIter copy = *this;
        return (copy += value);
    }

    constexpr auto operator-=(const DiffType value) noexcept -> SparseSetIter& {
        m_Offset += value;
        return *this;
    }

    constexpr auto operator-(const DiffType value) noexcept -> SparseSetIter {
        const SparseSetIter copy = *this;
        return (copy -= value);
    }

    [[nodiscard]] constexpr auto operator[](const DiffType value) const noexcept
        -> Ref {
        return (*m_Packed)[static_cast<Container::SizeType>(index() - value)];
    }

    [[nodiscard]] constexpr auto operator->() const noexcept -> Ptr {
        return addressof(operator[](0));
    }

    [[nodiscard]] constexpr auto operator*() const noexcept -> Ref {
        return operator[](0);
    }

    [[nodiscard]] constexpr auto
    operator-(const SparseSetIter& other) const noexcept -> ptrdiff_t {
        return other.m_Offset - m_Offset;
    }

    // [[nodiscard]] constexpr auto
    // operator==(const SparseSetIter& other) const noexcept -> bool {
    //     return other.m_Offset == m_Offset;
    // }

    [[nodiscard]] constexpr auto
    operator==(const SparseSetIter& other) const noexcept {
        return other.m_Offset <=> m_Offset;
    }

    [[nodiscard]] constexpr auto data() const noexcept -> Ptr {
        return m_Packed ? m_Packed->data() : nullptr;
    }

    [[nodiscard]] constexpr auto index() const noexcept -> DiffType {
        return m_Offset - 1;
    }

  private:
    const Container* m_Packed;
    DiffType m_Offset;
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