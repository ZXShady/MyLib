#pragma once

#ifndef HAD_TYPE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_HPP
#define HAD_TYPE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_HPP

#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

template<typename T,typename... Args>
struct is_nothrow_constructible : boolean_constant<__is_nothrow_constructible(T,Args...) > {};

#ifdef __cpp_variable_templates

template<typename T,typename... Args>
constexpr bool is_nothrow_constructible_v = __is_nothrow_constructible(T, Args...);

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_NOTHROW_CONSTRUCTIBLE_HPP
