#pragma once
#ifndef HAD_TYPE_TRAITS_IS_NOTHROW_SWAPPABLE
#define HAD_TYPE_TRAITS_IS_NOTHROW_SWAPPABLE

#include <had/version/namespace.hpp>
#include <had/utility/swap.hpp>
#include <had/utility/declval.hpp>
#include <had/type_traits/void_t.hpp>
#include <had/type_traits/special_member_functions.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/add_reference.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
struct is_nothrow_swappable : false_type {};

#ifdef __cpp_variable_templates

template<typename T>
constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_NOTHROW_SWAPPABLE
