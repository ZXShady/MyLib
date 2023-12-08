#pragma once
#ifndef HAD_TYPE_TRAITS_IS_POINTER_HPP
#define HAD_TYPE_TRAITS_IS_POINTER_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/remove_cv.hpp>
HAD_NAMESPACE_BEGIN


namespace details {
template<typename>   struct is_pointer     : HAD_NS false_type {};
template<typename T> struct is_pointer<T*> : HAD_NS true_type  {};

}

template<typename T> 
struct is_pointer : details::is_pointer<typename remove_cv<T>::type> {};

template<typename T> 
constexpr bool is_pointer_v = is_pointer<T>::value;

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_POINTER_HPP