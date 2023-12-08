#pragma once

#ifndef HAD_TYPE_TRAITS_IS_UNION_HPP
#define HAD_TYPE_TRAITS_IS_UNION_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

template<typename T>
constexpr static bool is_union = __is_union(T);

template<typename T>
struct is_union : boolean_constant<__is_union(T)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_UNION_HPP

