#pragma once

#ifndef HAD_TYPE_TRAITS_IS_ENUM_HPP
#define HAD_TYPE_TRAITS_IS_ENUM_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

template<typename T>
constexpr static auto is_enum_v = __is_enum(T);

template<typename T>
struct is_enum : boolean_constant<__is_enum(T)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_ENUM_HPP

