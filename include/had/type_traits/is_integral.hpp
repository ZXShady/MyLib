#pragma once

#ifndef HAD_TYPE_TRAITS_IS_INTEGRAL_HPP
#define HAD_TYPE_TRAITS_IS_INTEGRAL_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/remove_cv.hpp>
#include <had/type_traits/is_any_of.hpp>
HAD_NAMESPACE_BEGIN

template<typename>   struct is_integral                   : false_type {};
template<typename T> struct is_integral<const T>          : is_integral<T> {};
template<typename T> struct is_integral<volatile T>       : is_integral<T> {};
template<typename T> struct is_integral<const volatile T> : is_integral<T> {};

template<> struct is_integral<bool>               : true_type {};
template<> struct is_integral<char>               : true_type {};
#ifdef __cpp_char8_t
template<> struct is_integral<char8_t>            : true_type {};
#endif // __cpp_char8_t
template<> struct is_integral<char16_t>           : true_type {};
template<> struct is_integral<char32_t>           : true_type {};
template<> struct is_integral<wchar_t>            : true_type {};
template<> struct is_integral<signed char>        : true_type {};
template<> struct is_integral<signed short>       : true_type {};
template<> struct is_integral<signed int>         : true_type {};
template<> struct is_integral<signed long long>   : true_type {};
template<> struct is_integral<unsigned char>      : true_type {};
template<> struct is_integral<unsigned short>     : true_type {};
template<> struct is_integral<unsigned int>       : true_type {};
template<> struct is_integral<unsigned long long> : true_type {};

#ifdef __cpp_variable_templates

template<typename T> 
constexpr bool is_integral_v = is_integral<T>::value;


#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_INTEGRAL_HPP
