#pragma once

#ifndef HAD_TYPE_TRAITS_IS_BOUNDED_ARRAY_HPP
#define HAD_TYPE_TRAITS_IS_BOUNDED_ARRAY_HPP 1
#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

template<typename T>           struct is_bounded_array        : false_type {};
template<typename T>           struct is_bounded_array<T[]>   : false_type {};
template<typename T,size_t N>  struct is_bounded_array<T[N]>  : true_type  {};

#ifdef __cpp_variable_templates

template<typename T>
constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_BOUNDED_ARRAY_HPP
