#pragma once
#ifndef HAD_IS_MEMBER_FUNCTION_POINTER_HPP
#define HAD_IS_MEMBER_FUNCTION_POINTER_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/functional/function_traits.hpp>

HAD_NAMESPACE_BEGIN


#ifdef __cpp_variable_templates

#ifdef __clang__

template <typename FunctionPtr>
constexpr bool is_member_function_pointer_v = __is_member_function_pointer(FunctionPtr);

#else // !__clang__

template<typename FunctionPtr>
constexpr bool is_member_function_pointer_v = function_traits<FunctionPtr>::is_member_function;

#endif // __clang__

#endif // __cpp_variable_templates

HAD_NAMESPACE_END
#endif // !HAD_IS_MEMBER_FUNCTION_POINTER_HPP
