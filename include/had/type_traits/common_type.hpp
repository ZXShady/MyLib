#pragma once

#ifndef HAD_TYPE_TRAITS_COMMON_TYPE_HPP
#define HAD_TYPE_TRAITS_COMMON_TYPE_HPP
#include <had/version/namespace.hpp>
#include <had/utility/declval.hpp>
#include <had/type_traits/remove_cvref.hpp>
#include <had/type_traits/decay.hpp>
HAD_NAMESPACE_BEGIN

template<typename...>
struct common_type { using type = void; };

template<typename T>
struct common_type<T> {
    using type = T;
};


template<typename T1,typename T2,typename... Ts>
struct common_type<T1,T2,Ts...> : common_type<decltype(
    true ? HAD_NS declval<typename decay<typename remove_reference<T1>::type>::type>() : HAD_NS declval<typename decay<typename remove_reference<T2>::type>::type>()),Ts...> {};


template<typename T,typename U>
struct common_type<T,U> {
private:
    using decay_T = typename decay<typename remove_reference<T>::type>::type;
    using decay_U = typename decay<typename remove_reference<U>::type>::type;
public:
    using type = typename remove_reference<decltype(true ? HAD_NS declval<decay_T>() : HAD_NS declval<decay_U>())>::type;
};

#ifdef __cpp_alias_templates

template<typename... Ts>
using common_type_t = typename common_type<Ts...> ::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_COMMON_TYPE_HPP
