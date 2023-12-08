#pragma once

#ifndef HAD_TYPE_TRAITS_BOOLEAN_TYPES_HPP
#define HAD_TYPE_TRAITS_BOOLEAN_TYPES_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/integral_constant.hpp>

HAD_NAMESPACE_BEGIN

template<bool Bool>
struct boolean_constant : integral_constant<bool, Bool> {};

using true_type  = boolean_constant<true>;
using false_type = boolean_constant<false>;

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_BOOLEAN_TYPES_HPP
