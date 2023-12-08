#pragma once
#ifndef HAD_TYPE_TRAITS_IS_SIGNED_HPP
#define HAD_TYPE_TRAITS_IS_SIGNED_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <climits>

HAD_NAMESPACE_BEGIN 

template<typename> struct is_signed                     : false_type                     {};
template<>		   struct is_signed<char>               : boolean_constant<CHAR_MIN < 0> {};
template<>		   struct is_signed<signed char>        : true_type                      {};
template<>		   struct is_signed<signed short>       : true_type                      {};
template<>		   struct is_signed<signed int>         : true_type                      {};
template<>		   struct is_signed<signed long long>   : true_type                      {};

#ifdef __cpp_variable_templates

template<typename T>
constexpr bool is_signed_v = is_signed<T>::value;

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_SIGNED_HPP