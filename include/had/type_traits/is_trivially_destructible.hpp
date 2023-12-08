#pragma once
#ifndef HAD_TYPE_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_HPP
#define HAD_TYPE_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN 

template <typename T>
struct is_trivially_destructible : boolean_constant<__is_trivially_destructible(T)> {};

template <typename T>
constexpr static bool is_trivially_destructible_v = __is_trivially_destructible(T);

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_TRIVIALLY_DESTRUCTIBLE_HPP