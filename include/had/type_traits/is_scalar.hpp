#pragma once
#ifndef HAD_TYPE_TRAITS_SCALAR_HPP
#define HAD_TYPE_TRAITS_SCALAR_HPP
#include <had/type_traits/is_enum.hpp>
#include <had/type_traits/is_arithmetic.hpp>
#include <had/type_traits/is_pointer.hpp>
#include <had/type_traits/is_null_pointer.hpp>
#include <had/type_traits/is_member_function_pointer.hpp>


HAD_NAMESPACE_BEGIN

#ifdef __cpp_variable_templates


template<typename T>
constexpr bool is_scalar_v = is_arithmetic_v<T> 
                           || is_enum_v<T> 
                           || is_pointer_v<T> 
                           //|| is_member_pointer_v<T> 
                           || is_null_pointer_v<T>;

#endif // __cpp_variable_templates

template<typename T>
struct is_scalar : boolean_constant<
    is_arithmetic_v<T> 
    || is_enum_v<T> 
    || is_pointer_v<T> 
    //|| is_member_pointer_v<T> 
    || is_null_pointer_v<T>;
>  {};
HAD_NAMESPACE_END
#endif // !HAD_TYPE_TRAITS_SCALAR_HPP
