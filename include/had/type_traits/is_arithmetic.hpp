#pragma once
#ifndef HAD_TYPE_TRAITS_IS_ARITHMETIC_HPP
#define HAD_TYPE_TRAITS_IS_ARITHMETIC_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/is_floating_point.hpp>
#include <had/type_traits/is_integral.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
struct is_arithmetic : boolean_constant<is_floating_point<T>::value | is_integral<T>::value > {};

#ifdef __cpp_variable_templates

template<typename T>
constexpr bool is_arithmetic_v = is_floating_point_v<T> | is_integral_v<T>;

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_ARITHMETIC_HPP