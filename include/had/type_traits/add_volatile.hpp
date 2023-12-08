#pragma once

#ifndef HAD_TYPE_TRAITS_ADD_VOLATILE_HPP
#define HAD_TYPE_TRAITS_ADD_VOLATILE_HPP 1

#include <had/version/namespace.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
struct add_volatile { using type = volatile T; };

template<typename T>
struct add_volatile_forced { using type = volatile T; };
template<typename T>
struct add_volatile_forced<T&> { using type = volatile T&; };
template<typename T>
struct add_volatile_forced<T&&> { using type = volatile T&&; };

#ifdef __cpp_alias_templates

template<typename T>
using add_volatile_t = typename add_volatile<T>::type;

template<typename T>
using add_volatile_forced_t = typename add_volatile_forced<T>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_ADD_VOLATILE_HPP
