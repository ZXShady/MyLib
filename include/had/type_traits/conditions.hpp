#pragma once

#ifndef HAD_TYPE_TRAITS_CONDITIONS_HPP
#define HAD_TYPE_TRAITS_CONDITIONS_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

template<typename T> struct if_const          : false_type {};
template<typename T> struct if_const<const T> : true_type  {};

template<typename T> struct if_volatile             : false_type {};
template<typename T> struct if_volatile<volatile T> : true_type  {};


template<typename T>
constexpr static bool if_const_v = if_const<T>::value;


template<typename T>
constexpr static bool if_volatile_v = if_volatile<T>::value;



HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_CONDITIONS_HPP
