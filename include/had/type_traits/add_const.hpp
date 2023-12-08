#pragma once

#ifndef HAD_TYPE_TRAITS_ADD_CONST_HPP
#define HAD_TYPE_TRAITS_ADD_CONST_HPP
#include <had/version/namespace.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
struct add_const { using type = const T; };

template<typename T>
struct add_const_forced { using type = const T; };
template<typename T>
struct add_const_forced<T&> { using type = const T&; };
template<typename T>
struct add_const_forced<T&&> { using type = const T&&; };

#ifdef __cpp_alias_templates

template<typename T>
using add_const_t = typename add_const<T>::type;

template<typename T>
using add_const_forced_t = typename add_const_forced<T>::type;

#endif // __cpp_alias_templates


HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_ADD_CONST_HPP
