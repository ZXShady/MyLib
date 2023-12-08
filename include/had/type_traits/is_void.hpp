#pragma once
#ifndef HAD_TYPE_TRAITS_IS_VOID_HPP
#define HAD_TYPE_TRAITS_IS_VOID_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/remove_cv.hpp>
#include <had/type_traits/is_same.hpp>
HAD_NAMESPACE_BEGIN


template<typename T>
constexpr static bool is_void_v = is_same_v<remove_cv_t<T>,void>;

template<typename T> 
struct is_void : boolean_constant<is_void_v<T>> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_VOID_HPP