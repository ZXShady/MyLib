#pragma once

#ifndef HAD_TYPE_TRAITS_IS_OBJECT_HPP
#define HAD_TYPE_TRAITS_IS_OBJECT_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/is_const.hpp>
#include <had/type_traits/is_void.hpp>

HAD_NAMESPACE_BEGIN

template<typename T>
constexpr static bool is_object_v = is_const_v<const T> && !is_void_v<T>;

template<typename T>
struct is_object : boolean_constant<is_object_v<T>> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_OBJECT_HPP
