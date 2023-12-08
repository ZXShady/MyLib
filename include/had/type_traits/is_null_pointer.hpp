#pragma once
#ifndef HAD_TYPE_TRAITS_IS_NULL_POINTER_HPP
#define HAD_TYPE_TRAITS_IS_NULL_POINTER_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/is_same.hpp>
#include <had/type_traits/remove_cv.hpp>

HAD_NAMESPACE_BEGIN

template<typename T> struct is_null_pointer : boolean_constant<is_same_no_qualifiers<T,decltype(nullptr)>::value> {};

#ifdef __cpp_alias_templates

template<typename T> 
constexpr bool is_null_pointer_v = is_same_no_qualifiers_v<T, decltype(nullptr)>;

#endif // __cpp_alias_templates
HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_NULL_POINTER_HPP