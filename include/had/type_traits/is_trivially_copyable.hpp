#pragma once

#ifndef HAD_TYPE_TRAITS_IS_TRIVIALLY_COPYABLE_HPP
#define HAD_TYPE_TRAITS_IS_TRIVIALLY_COPYABLE_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

#ifdef __cpp_variable_templates

template <typename T>
constexpr static bool is_trivially_copyable_v = __is_trivially_copyable(T);

#endif // __cpp_variable_templates

template<typename T>
struct is_trivially_copyable : boolean_constant<__is_trivially_copyable(T)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_TRIVIALLY_COPYABLE_HPP
