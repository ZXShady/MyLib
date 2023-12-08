#pragma once

#ifndef HAD_TYPE_TRAITS_IS_ARRAY_HPP
#define HAD_TYPE_TRAITS_IS_ARRAY_HPP 1
#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>          struct is_array        : false_type {};
template<typename T,size_t N> struct is_array<T[N]>  : true_type  {};
template<typename T>          struct is_array<T[]>   : true_type  {};

#ifdef __cpp_variable_templates

template<typename T>
constexpr bool is_array_v = is_array<T>::value;

#endif // __cpp_variable_templates


HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_ARRAY_HPP
