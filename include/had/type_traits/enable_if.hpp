#pragma once

#ifndef HAD_TYPE_TRAITS_ENABLE_IF_HPP
#define HAD_TYPE_TRAITS_ENABLE_IF_HPP 1

#include <had/version/namespace.hpp>

HAD_NAMESPACE_BEGIN

template<bool B,typename T = int>
struct enable_if {}; // nothing in there causes syntax error!

template<typename T>
struct enable_if<true,T> { // type is void* not void cuz void is not valid type
    using type = T;
};

#ifdef __cpp_alias_templates

template<bool B,typename T = int>
using enable_if_t = typename enable_if<B,T>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_ENABLE_IF_HPP
