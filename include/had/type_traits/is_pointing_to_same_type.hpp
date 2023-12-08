#pragma once
#ifndef HAD_TYPE_TRAITS_IS_POINTING_TO_SAME_TYPE_HPP
#define HAD_TYPE_TRAITS_IS_POINTING_TO_SAME_TYPE_HPP

#include <had/type_traits/remove_cv.hpp>
#include <had/type_traits/add_pointer.hpp>
#include <had/type_traits/is_same.hpp>

HAD_NAMESPACE_BEGIN

namespace details {

template<typename T,typename U>
struct is_pointing_to_same_type : false_type {};

template<typename T, typename U>
struct is_pointing_to_same_type<T*,U*> : boolean_constant<is_same<T,U>::value> {};

template<typename T,typename U>
struct is_pointing_to_same_type_no_qualifiers : false_type {};

template<typename T, typename U>
struct is_pointing_to_same_type_no_qualifiers<T*,U*> : boolean_constant<is_same_no_qualifiers<T,U>::value> {};


}

template<typename T,typename U>
struct is_pointing_to_same_type : details::is_pointing_to_same_type<
    typename remove_cv<T>::type,typename remove_cv<T>::type> {};

template<typename T, typename U>
struct is_pointing_to_same_type_no_qualifiers : details::is_pointing_to_same_type_no_qualifiers<
    typename remove_cv<T>::type,typename remove_cv<U>::type> {};


#ifdef __cpp_variable_templates


template<typename T, typename U>
constexpr bool is_pointing_to_same_type_v = is_pointing_to_same_type<T, U>::value;


template<typename T, typename U>
constexpr bool is_pointing_to_same_type_no_qualifiers_v = is_pointing_to_same_type_no_qualifiers<T, U>::value;

#endif // __cpp_variable_templates

HAD_NAMESPACE_END
#endif // !HAD_TYPE_TRAITS_IS_POINTING_TO_SAME_TYPE_HPP
