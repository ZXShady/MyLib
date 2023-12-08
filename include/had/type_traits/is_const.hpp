#pragma once

#ifndef HAD_TYPE_TRAITS_IS_CONST_HPP
#define HAD_TYPE_TRAITS_IS_CONST_HPP 1

#include <had/version/namespace.hpp>
#include <had/version/attributes/inline_varaible.hpp>
#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

template<typename>   struct is_const          : false_type {};
template<typename T> struct is_const<const T> : true_type {};


#ifdef __cpp_variable_templates


template<typename>
HAD_INLINE_VAR constexpr bool is_const_v = false;

template<typename T>
HAD_INLINE_VAR constexpr bool is_const_v<const T> = true;

#endif // __cpp_variable_templates


HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_CONST_HPP