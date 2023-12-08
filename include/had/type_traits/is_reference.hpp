#pragma once
#ifndef HAD_TYPE_TRAITS_IS_REFERENCE_HPP
#define HAD_TYPE_TRAITS_IS_REFERENCE_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/remove_cv.hpp>

HAD_NAMESPACE_BEGIN 

template<typename T> struct is_reference                           : false_type {};
template<typename T> struct is_reference<T&>          : true_type  {};
template<typename T> struct is_reference<T&&>         : true_type  {};
template<typename T> struct is_lvalue_reference                    : false_type {};
template<typename T> struct is_lvalue_reference<T&>   : true_type  {};
template<typename T> struct is_rvalue_reference                    : false_type {};
template<typename T> struct is_rvalue_reference<T&&>  : true_type  {};

template<typename T>
constexpr static bool is_reference_v = is_reference<remove_cv_t<T>>::value;

template<typename T>
constexpr static bool is_lvalue_reference_v = is_lvalue_reference<remove_cv_t<T>>::value;

template<typename T>
constexpr static bool is_rvalue_reference_v = is_rvalue_reference<remove_cv_t<T>>::value;

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_REFERENCE_HPP