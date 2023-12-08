#pragma once

#ifndef HAD_TYPE_TRAITS_REMOVE_CVREF_HPP
#define HAD_TYPE_TRAITS_REMOVE_CVREF_HPP 1
#include <had/version/namespace.hpp>
#include <had/type_traits/remove_cv.hpp>
#include <had/type_traits/remove_reference.hpp>

HAD_NAMESPACE_BEGIN

template<typename T>
struct remove_cvref {
    using type = typename remove_cv<typename remove_reference<T>::type>::type;
};

#ifdef __cpp_alias_templates

template<typename T>
using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_REMOVE_CVREF_HPP
