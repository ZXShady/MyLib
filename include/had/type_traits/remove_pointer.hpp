#pragma once

#ifndef HAD_TYPE_TRAITS_REMOVE_POINTER_HPP
#define HAD_TYPE_TRAITS_REMOVE_POINTER_HPP 1
#include <had/version/namespace.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
struct remove_pointer { using type = T; };

template<typename T>
struct remove_pointer<T*> { using type = T; };

template<typename T>
struct remove_pointer<T* const> { using type = T; };

template<typename T>
struct remove_pointer<T* volatile> { using type = T; };

template<typename T>
struct remove_pointer<T* const volatile> { using type = T; };

#ifdef __cpp_alias_templates

template<typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

#endif // defined(__cpp_alias_templates)
HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_REMOVE_POINTER_HPP
