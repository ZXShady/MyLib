#pragma once

#ifndef HAD_TYPE_TRAITS_IS_TRIVIALLY_COPY_ASSIGNABLE_HPP
#define HAD_TYPE_TRAITS_IS_TRIVIALLY_COPY_ASSIGNABLE_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/add_reference.hpp>
HAD_NAMESPACE_BEGIN

#ifdef __cpp_variable_templates


template<typename T>
HAD_INLINE_VAR constexpr bool is_trivially_copy_assignable_v = __is_trivially_assignable(
    add_lvalue_reference_t<T>,
    add_lvalue_reference_t<const T>
);

#endif // __cpp_variable_templates

template <typename T>
struct is_trivially_copy_assignable : boolean_constant<__is_trivially_assignable(
    typename add_lvalue_reference<T>::type,
    typename add_lvalue_reference<const T>::type
)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_TRIVIALLY_COPY_ASSIGNABLE_HPP
