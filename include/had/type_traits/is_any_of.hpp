#pragma once

#ifndef HAD_TYPE_TRAITS_IS_ANY_OF_HPP
#define HAD_TYPE_TRAITS_IS_ANY_OF_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/integral_constant.hpp>
#include <had/type_traits/is_same.hpp>
#include <had/type_traits/disjunction.hpp>
HAD_NAMESPACE_BEGIN

template<typename T,typename... Ts>
struct is_any_of : boolean_constant<disjunction<is_same<T,Ts>...>::value> {};

#ifdef __cpp_variable_templates

template<typename T,typename... Ts>
constexpr bool is_any_of_v = disjunction_v<is_same<T,Ts>...>;

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_ANY_OF_HPP