#pragma once
#ifndef HAD_TYPE_TRAITS_EXTRACT_HPP
#define HAD_TYPE_TRAITS_EXTRACT_HPP
#include <had/version/namespace.hpp>
#include <had/type_traits/conditional.hpp>
HAD_NAMESPACE_BEGIN

namespace details {
template<size_t Index,typename,typename... Rest>
struct extract {
    using type = typename extract<Index - 1,Rest...>::type;
};

template<typename First,typename... Rest>
struct extract<0,First,Rest...> {
    using type = First;
};

}


template<size_t Index,typename... Ts>
struct extract {
    static_assert(Index < sizeof...(Ts), "had::extract<Index,Types...> Index out of boundes");
    using type = typename details::extract<Index, Ts...>::type;
};

#ifdef __cpp_alias_templates

template<size_t I,typename... Ts>
using extract_t = typename extract<I,Ts...>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_EXTRACT_HPP