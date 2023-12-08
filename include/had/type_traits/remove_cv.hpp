#pragma once

#ifndef HAD_TYPE_TRAITS_REMOVE_CV_HPP
#define HAD_TYPE_TRAITS_REMOVE_CV_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/remove_const.hpp>
#include <had/type_traits/remove_volatile.hpp>
HAD_NAMESPACE_BEGIN
template<typename T>
struct remove_cv {
    using type = T;
};

template<typename T>
struct remove_cv<const T> {
    using type = T;
};

template<typename T>
struct remove_cv<volatile T> {
    using type = T;
};

template<typename T>
struct remove_cv<const volatile T> {
    using type = T;
};

#ifdef __cpp_alias_templates


template<typename T>
using remove_cv_t = remove_volatile_t<remove_const_t<T>>;

#endif // __cpp_alias_templates
HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_REMOVE_CV_HPP