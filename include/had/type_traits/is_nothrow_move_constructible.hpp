#pragma once

#ifndef HAD_TYPE_TRAITS_IS_NOTHROW_MOVE_CONSTRUCTIBLE_HPP
#define HAD_TYPE_TRAITS_IS_NOTHROW_MOVE_CONSTRUCTIBLE_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/inline_varaible.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN


template<typename T>
struct is_nothrow_move_constructible : boolean_constant<__is_nothrow_constructible(T, T)> {};

#ifdef __cpp_variable_templates

template <typename T>
constexpr bool is_nothrow_move_constructible_v = __is_nothrow_constructible(T, T);

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_NOTHROW_MOVE_CONSTRUCTIBLE_HPP
