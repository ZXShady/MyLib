#pragma once

#ifndef HAD_TYPE_TRAITS_IS_NOTHROW_COPY_CONSTRUCTIBLE_HPP
#define HAD_TYPE_TRAITS_IS_NOTHROW_COPY_CONSTRUCTIBLE_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/inline_varaible.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

#ifdef __cpp_variable_templates

template <typename T>
constexpr bool is_nothrow_copy_constructible_v = __is_nothrow_constructible(T, T);

#endif // HAD_CPP14

template<typename T>
struct is_nothrow_copy_constructible : boolean_constant<__is_nothrow_constructible(T, T)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_NOTHROW_COPY_CONSTRUCTIBLE_HPP
