#pragma once

#ifndef HAD_TYPE_TRAITS_IS_FUNCTION_HPP
#define HAD_TYPE_TRAITS_IS_FUNCTION_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/is_const.hpp>
#include <had/type_traits/is_reference.hpp>

HAD_NAMESPACE_BEGIN

#ifdef __cpp_variable_templates

template <typename T>
constexpr bool is_function_v = !is_const_v<const T> && !is_reference_v<T>;

#endif

template <typename T>
struct is_function : boolean_constant<!is_const<const T>::value && !is_reference<T>::value> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_FUNCTION_HPP