#pragma once

#ifndef HAD_TYPE_TRAITS_IS_NOTHROW_MOVE_ASSIGNABLE_HPP
#define HAD_TYPE_TRAITS_IS_NOTHROW_MOVE_ASSIGNABLE_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/add_reference.hpp>

HAD_NAMESPACE_BEGIN
template <typename T>
constexpr static bool is_nothrow_move_assignable_v = __is_nothrow_assignable(add_lvalue_reference_t<T>, T);

template<typename T>
struct is_nothrow_move_assignable : boolean_constant< __is_nothrow_assignable(typename add_lvalue_reference<T>::type, T)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_NOTHROW_MOVE_ASSIGNABLE_HPP
