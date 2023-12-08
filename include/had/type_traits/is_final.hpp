#pragma once

#ifndef HAD_TYPE_TRAITS_IS_FINAL_HPP
#define HAD_TYPE_TRAITS_IS_FINAL_HPP 1
#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

#ifdef __cpp_variable_templates


template<typename T>
constexpr bool is_final_v = __is_final(T);

#endif // __cpp_variable_templates

template<typename T>
struct is_final : boolean_constant<__is_final(T)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_FINAL_HPP
