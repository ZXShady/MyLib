#pragma once
#ifndef HAD_TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP
#define HAD_TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/version/attributes/inline_varaible.hpp>
HAD_NAMESPACE_BEGIN 

template <class T, class... Args>
struct is_constructible : boolean_constant<__is_constructible(T, Args...)> {
// determine whether _Ty can be direct-initialized with _Args...
};

#ifdef __cpp_variable_templates

template <class T, class... Args>
constexpr bool is_constructible_v = __is_constructible(T, Args...);

#endif // __cpp_variable_templates
HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP