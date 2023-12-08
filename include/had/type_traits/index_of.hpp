#pragma once
#ifndef HAD_TYPE_TRAITS_INDEX_OF_HPP
#define HAD_TYPE_TRAITS_INDEX_OF_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/is_same.hpp>
#include <had/type_traits/conditional.hpp>
HAD_NAMESPACE_BEGIN


namespace details {

template<size_t I,typename...>
struct index_of {

};

template<size_t I,typename T,typename U>
struct index_of<I,T,U> : HAD_NS integral_constant<size_t,(is_same_v<T,U>) ? I : size_t(-1)> {};
template<size_t I,typename T,typename U,typename... Ts>
struct index_of<I,T,U,Ts...> : HAD_NS conditional_t<HAD_NS is_same_v<T,U>,HAD_NS integral_constant<size_t,I>,index_of<I + 1,T,Ts...> > {
    
};

}

template<typename T,typename... Ts>
struct index_of : integral_constant<size_t, details::index_of<0, T, Ts...>::value > {
};

#ifdef __cpp_alias_templates

template<typename T,typename... Ts>
constexpr size_t index_of_v = index_of<T, Ts...>::value;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_INDEX_OF_HPP
