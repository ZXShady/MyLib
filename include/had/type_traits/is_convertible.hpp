#pragma once

#ifndef HAD_TYPE_TRAITS_IS_CONVERTIBLE_HPP
#define HAD_TYPE_TRAITS_IS_CONVERTIBLE_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

template <class From, class To>
struct is_convertible : boolean_constant<__is_convertible_to(From, To)> {};

#ifdef __cpp_variable_templates

template <class From, class To>
constexpr bool is_convertible_v = __is_convertible_to(From, To);

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_CONVERTIBLE_HPP
