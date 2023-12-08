#pragma once

#ifndef HAD_TYPE_TRAITS_BIGGEST_SIZEOF_HPP
#define HAD_TYPE_TRAITS_BIGGEST_SIZEOF_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/integral_constant.hpp>
#include <had/type_traits/conditional.hpp>
HAD_NAMESPACE_BEGIN

template<typename...>
struct biggest_sizeof {};


template<typename T>
struct biggest_sizeof<T> : integral_constant<size_t,sizeof(T)> {
    using type = T;
};

template<typename T,typename U>
struct biggest_sizeof<T,U> : integral_constant<size_t,
    (sizeof(T) > sizeof(U)) ? sizeof(T) : sizeof(U)> {
    using type = typename conditional<(sizeof(T) > sizeof(U)), T, U>::type;
};

template<typename T,typename U,typename... Ts>
struct biggest_sizeof<T,U,Ts...> : conditional_t<(sizeof(T) > sizeof(U)),biggest_sizeof<T,Ts...>,biggest_sizeof<U,Ts...> >  {};

#ifdef __cpp_variable_templates

template<typename... Ts>
constexpr size_t biggest_sizeof_v = biggest_sizeof<Ts...>::value;

#endif // __cpp_variable_templates

#ifdef __cpp_alias_templates

template<typename... Ts>
using biggest_sizeof_t = typename biggest_sizeof<Ts...>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_BIGGEST_SIZEOF_HPP