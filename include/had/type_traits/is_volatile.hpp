#pragma once

#ifndef HAD_TYPE_TRAITS_IS_VOLATILE_HPP
#define HAD_TYPE_TRAITS_IS_VOLATILE_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/is_same.hpp>
HAD_NAMESPACE_BEGIN


template<typename>
constexpr static bool is_volatile_v = false;

template<typename T>
constexpr static bool is_volatile_v<volatile T> = true;

template<typename T>
struct is_volatile : boolean_constant<is_volatile_v<T>> {};
HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_VOLATILE_HPP