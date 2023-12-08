#pragma once

#ifndef HAD_TYPE_TRAITS_IS_MOVE_CONSTRUCTIBLE_HPP
#define HAD_TYPE_TRAITS_IS_MOVE_CONSTRUCTIBLE_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/version/attributes/inline_varaible.hpp>
HAD_NAMESPACE_BEGIN

#ifdef __cpp_variable_templates

template <typename T>
HAD_INLINE_VAR constexpr bool is_move_constructible_v = __is_constructible(T, T);

#endif // __cpp_variable_templates

template<typename T>
struct is_move_constructible : boolean_constant<__is_constructible(T, T)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_MOVE_CONSTRUCTIBLE_HPP
