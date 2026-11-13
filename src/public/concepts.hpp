#pragma once

#include <concepts>

namespace bt::types {

template<typename T, typename... Args>
concept ConstructableFrom = std::constructible_from<T, Args...>;

template<typename T>
concept DefaultInitializable = std::default_initializable<T>;

template<typename Derived, typename Base>
concept DerivedFrom = std::derived_from<Derived, Base>;

template<typename T>
concept Integral = std::integral<T>;

template<typename T, typename... Args>
concept Invocable = std::invocable<T, Args...>;

template<typename T1, typename T2>
concept SameAs = std::same_as<T1, T2>;

template<typename T>
concept UnsignedIntegral = std::unsigned_integral<T>;

} // namespace bt::types
