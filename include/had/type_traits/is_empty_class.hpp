#pragma once

#ifndef HAD_TYPE_TRAITS_IS_EMPTY_CLASS_HPP
#define HAD_TYPE_TRAITS_IS_EMPTY_CLASS_HPP 1
#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
constexpr static bool is_empty_class_v = __is_empty(T);

template<typename T>
struct is_empty_class : boolean_constant<__is_empty(T)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_EMPTY_CLASS_HPP
