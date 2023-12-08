#pragma once
#ifndef HAD_TYPE_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_HPP
#define HAD_TYPE_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/add_reference.hpp>

HAD_NAMESPACE_BEGIN


template <typename T>
struct is_trivially_move_assignable : boolean_constant<
    __is_trivially_assignable(typename add_lvalue_reference<T>::type, T)> {};

#ifdef __cpp_variable_templates

template<typename T>
constexpr bool is_trivially_move_assignable_v = __is_trivially_assignable(typename add_lvalue_reference<T>::type , T);

#endif // __cpp_varaible_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_TRIVIALLY_MOVE_ASSIGNABLE_HPP
