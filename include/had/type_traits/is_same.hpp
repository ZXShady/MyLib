#pragma once
#ifndef HAD_TYPE_TRAITS_IS_SAME_HPP
#define HAD_TYPE_TRAITS_IS_SAME_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/remove_cv.hpp>

HAD_NAMESPACE_BEGIN 
#ifdef __clang__
template<typename T, typename U> struct is_same : boolean_constant<__is_same(T,U)> {};
#ifdef __cpp_variable_templates

template<typename T, typename U>
constexpr bool is_same_v = __is_same(T, U);

#endif // __cpp_variable_templates


#else 
template<typename T, typename U> struct is_same          : false_type {};
template<typename T>             struct is_same<T, T>    : true_type  {};
template<typename T, typename U> struct is_same_no_cv    : is_same<typename remove_cvref<T>::type,typename remove_cvref<U>::type> {};
template<typename T, typename U> struct is_same_no_cvref : is_same<typename remove_cvref<T>::type,typename remove_cvref<U>::type> {};


#if defined(__cpp_variable_templates) && !defined(__clang__)
template<typename T, typename U>
constexpr bool is_same_v = is_same<T, U>::value;
template<typename T,typename U>
constexpr bool is_same_no_cv_v = is_same_no_cv<T,U>::value;
template<typename T,typename U>
constexpr bool is_same_no_cvref_v = is_same_no_cvref<T,U>::value;

#elif defined(__cpp_variable_templates) && defined(__clang__)

template<typename T, typename U>
constexpr bool is_same_v = __is_same(T, U);
template<typename T,typename U>
constexpr bool is_same_no_cv_v = __is_same(remove_cv_t<T>,remove_cv_t<U>);
template<typename T,typename U>
constexpr bool is_same_no_cvref_v = __is_same(remove_cvref_t<T>,remove_cvref_t<U>);

#endif // __cpp_variable_templates




HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_SAME_HPP