#pragma once

#ifndef HAD_TYPE_TRAITS_REMOVE_ALL_POINTERS_HPP
#define HAD_TYPE_TRAITS_REMOVE_ALL_POINTERS_HPP
#include <had/version/namespace.hpp>
#include <had/type_traits/remove_pointer.hpp>
#include <had/type_traits/remove_cv.hpp>
#include <had/type_traits/type_identity.hpp>
HAD_NAMESPACE_BEGIN

namespace details {


template<typename T> struct remove_all_pointers : type_identity<T> {};
template<typename T> struct remove_all_pointers<T*> : remove_all_pointers<T> {};
template<typename T> struct remove_all_pointers<T*const> : remove_all_pointers<T> {};
template<typename T> struct remove_all_pointers<T*volatile> : remove_all_pointers<T> {};
template<typename T> struct remove_all_pointers<T*const volatile> : remove_all_pointers<T> {};

#ifdef __cpp_alias_templates

template<typename T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_REMOVE_ALL_POINTERS_HPP
