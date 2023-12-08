#pragma once
#ifndef HAD_TYPE_TRAITS_ALWAYS_FALSE_HPP
#define HAD_TYPE_TRAITS_ALWAYS_FALSE_HPP

#include <had/version/namespace.hpp>

#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

template<typename>
struct always_false : false_type {};

#ifdef __cpp_variable_templates

template<typename T>
constexpr bool always_false_v = always_false<T>::value;

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_ALWAYS_FALSE_HPP
