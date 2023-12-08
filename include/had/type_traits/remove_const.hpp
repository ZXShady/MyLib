#pragma once

#ifndef HAD_TYPE_TRAITS_REMOVE_CONST_HPP
#define HAD_TYPE_TRAITS_REMOVE_CONST_HPP 1

#include <had/version/namespace.hpp>

HAD_NAMESPACE_BEGIN

template<typename T> struct remove_const { using type = T; };
template<typename T> struct remove_const<const T> { using type = T; };

template <typename T>
using remove_const_t = typename remove_const<T>::type;

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_REMOVE_CONST_HPP