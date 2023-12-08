#pragma once

#ifndef HAD_TYPE_TRAITS_REMOVE_VOLATILE_HPP
#define HAD_TYPE_TRAITS_REMOVE_VOLATILE_HPP 1

#include <had/version/namespace.hpp>

HAD_NAMESPACE_BEGIN

template<typename T> struct remove_volatile             { using type = T; };
template<typename T> struct remove_volatile<volatile T> { using type = T; };

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_REMOVE_VOLATILE_HPP