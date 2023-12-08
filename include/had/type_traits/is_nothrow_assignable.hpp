#pragma once

#ifndef HAD_TYPE_TRAITS_IS_NOTHROW_ASSIGNABLE_HPP
#define HAD_TYPE_TRAITS_IS_NOTHROW_ASSIGNABLE_HPP

#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

template<typename To,typename From>
struct is_nothrow_assignable : boolean_constant<__is_nothrow_assignable(To,From) > {};

#ifdef __cpp_variable_templates

template<typename To,typename From>
constexpr bool is_nothrow_assignable_v = __is_nothrow_assignable(To, From);
#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_NOTHROW_ASSIGNABLE_HPP
