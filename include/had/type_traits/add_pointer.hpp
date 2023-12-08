#pragma once

#ifndef HAD_TYPE_TRAITS_ADD_POINTER_HPP
#define HAD_TYPE_TRAITS_ADD_POINTER_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/void_t.hpp>

HAD_NAMESPACE_BEGIN

template<typename T,typename = void>
struct add_pointer { using type = T; };

template<typename T>
struct add_pointer<T,typename void_s<T*>::type> { using type = T*; };

#ifdef __cpp_alias_templates

template<typename T>
using add_pointer_t = typename add_pointer<T>::type;

#endif // __cpp_alias_templates


HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_ADD_POINTER_HPP
