#pragma once

#ifndef HAD_TYPE_TRAITS_ADD_REFERENCE_HPP
#define HAD_TYPE_TRAITS_ADD_REFERENCE_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/void_t.hpp>
#include <had/type_traits/remove_reference.hpp>
HAD_NAMESPACE_BEGIN

template<typename T,typename = void>
struct add_reference {
    using lvalue = T;
    using rvalue = T;
};
template<typename T>
struct add_reference<T,typename void_s<T&>::type> {
    using lvalue = typename remove_reference<T>::type&;
    using rvalue = typename remove_reference<T>::type&&;
};

template<typename T,typename = void>
struct add_lvalue_reference {
    using lvalue = T;
    using type = lvalue;
};

template<typename T>
struct add_lvalue_reference<T,typename void_s<T&>::type> {
    using lvalue = typename remove_reference<T>::type&;
    using type = lvalue;
};

template<typename T,typename = void>
struct add_rvalue_reference {
    using rvalue = T;
    using type   = T;
};

template<typename T>
struct add_rvalue_reference<T,typename void_s<T&&>::type> {
    using rvalue = typename remove_reference<T>::type&&;
    using type = rvalue;
};

#ifdef __cpp_alias_templates

template<typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::lvalue;

template<typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::rvalue;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_ADD_REFERENCE_HPP
