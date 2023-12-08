#pragma once

#ifndef HAD_TYPE_TRAITS_ADD_CV_HPP
#define HAD_TYPE_TRAITS_ADD_CV_HPP 1

#include <had/version/namespace.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
struct add_cv { using type = const volatile T; };


#ifdef __cpp_alias_templates

template<typename T>
using add_cv_t = typename add_cv<T>::type;

#endif // __cpp_alias_templates



HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_ADD_CV_HPP
