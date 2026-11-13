#pragma once

#include <type_traits>

namespace bt::types {

template<bool Val>
using BoolConst = std::bool_constant<Val>;

template<typename... T>
using CommonTypeT = std::common_type_t<T...>;

template<bool Test, typename T1, typename T2>
using CondType = std::conditional_t<Test, T1, T2>;

template<typename T>
using DecayType = std::decay_t<T>;

using std::extent_v;

using FalseType = std::false_type;

template<class T, T Val>
using IntegralConst = std::integral_constant<T, Val>;

template<class Callable, class... Args>
using InvokeResultType = std::invoke_result_t<Callable, Args...>;

using std::is_aggregate_v;
using std::is_arithmetic_v;
using std::is_array_v;
using std::is_base_of_v;
using std::is_class_v;
using std::is_const_v;
using std::is_constructible_v;
using std::is_convertible_v;
using std::is_copy_assignable_v;
using std::is_copy_constructible_v;
using std::is_default_constructible_v;
using std::is_empty_v;
using std::is_enum_v;
using std::is_final_v;
using std::is_function_v;
using std::is_integral_v;
using std::is_invocable_r_v;
using std::is_invocable_v;
using std::is_lvalue_reference_v;
using std::is_member_function_pointer_v;
using std::is_member_object_pointer_v;
using std::is_member_pointer_v;
using std::is_move_assignable_v;
using std::is_move_constructible_v;
using std::is_nothrow_constructible_v;
using std::is_nothrow_copy_constructible_v;
using std::is_nothrow_default_constructible_v;
using std::is_nothrow_destructible_v;
using std::is_nothrow_invocable_v;
using std::is_nothrow_move_constructible_v;
using std::is_pointer_v;
using std::is_reference_v;
using std::is_same_v;
using std::is_signed_v;
using std::is_trivially_destructible_v;
using std::is_void_v;
using std::remove_const_t;
using std::remove_cvref_t;
using std::remove_pointer_t;
using std::remove_reference_t;

using TrueType = std::true_type;

template<typename T>
using TypeId = std::type_identity<T>;

template<typename T>
using UnderlyingType = std::underlying_type_t<T>;

} // namespace bt::types