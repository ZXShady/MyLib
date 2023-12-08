#pragma once

#ifndef HAD_TYPE_TRAITS_IS_FUNDAMENTAL_HPP
#define HAD_TYPE_TRAITS_IS_FUNDAMENTAL_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/integral_constant.hpp>
#include <had/type_traits/is_arithmetic.hpp>
#include <had/type_traits/is_null_pointer.hpp>
#include <had/type_traits/is_void.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
static constexpr bool is_fundamental_v =  is_arithmetic_v<T> 
                                       or is_void_v<T>
                                       or is_null_pointer_v<T>;

template<typename T>
struct is_fundamental : boolean_constant<is_fundamental_v<T>> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_FUNDAMENTAL_HPP