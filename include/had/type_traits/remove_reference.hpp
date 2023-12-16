#pragma once

#ifndef HAD_TYPE_TRAITS_REMOVE_REFERENCE_HPP
#define HAD_TYPE_TRAITS_REMOVE_REFERENCE_HPP 1

#include <had/version/namespace.hpp>

HAD_NAMESPACE_BEGIN

template<typename T> struct remove_reference      { using type = T; };
template<typename T> struct remove_reference<T&>  { using type = T; };
template<typename T> struct remove_reference<T&&> { using type = T; };

template<typename T> struct remove_lvalue_reference      { using type = T; };
template<typename T> struct remove_lvalue_reference<T&>  { using type = T; };

template<typename T> struct remove_rvalue_reference      { using type = T; };
template<typename T> struct remove_rvalue_reference<T&&> { using type = T; };

#ifdef __cpp_alias_templates

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;
template <typename T>
using remove_lvalue_reference_t = typename remove_lvalue_reference<T>::type;
template <typename T>
using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_REMOVE_REFERENCE_HPP