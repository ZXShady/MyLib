#pragma once
#ifndef HAD_TYPE_TRAITS_IS_UNSIGNED_HPP
#define HAD_TYPE_TRAITS_IS_UNSIGNED_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/version/attributes/inline_varaible.hpp>
#include <climits>

HAD_NAMESPACE_BEGIN 

template<typename> struct is_unsigned                     : false_type                      {};
template<>		   struct is_unsigned<char>               : boolean_constant<CHAR_MIN >= 0> {};
template<>		   struct is_unsigned<unsigned char>      : true_type                       {};
template<>		   struct is_unsigned<unsigned short>     : true_type                       {};
template<>		   struct is_unsigned<unsigned int>       : true_type                       {};
template<>		   struct is_unsigned<unsigned long long> : true_type                       {};

#ifdef __cpp_variable_templates

template<typename T>
HAD_INLINE_VAR constexpr bool is_unsigned_v = is_unsigned<T>::value;

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_UNSIGNED_HPP