#pragma once

#pragma once

#ifndef HAD_TYPE_TRAITS_IS_RANDOM_ACCESS_ITERATOR_HPP
#define HAD_TYPE_TRAITS_IS_RANDOM_ACCESS_ITERATOR_HPP
#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/is_same.hpp>
#include <had/type_traits/is_pointer.hpp>
#include <had/type_traits/add_pointer.hpp>
#include <had/type_traits/is_member_function_pointer.hpp>
#include <had/utility/declval.hpp>
#include <had/type_traits/is_class.hpp>
#include <had/type_traits/is_void.hpp>
#include <had/type_traits/remove_cv.hpp>
HAD_NAMESPACE_BEGIN

template<typename T,bool = is_class_v<T>>
struct is_random_access_iterator : false_type
{ 
};

template<typename T>
struct is_random_access_iterator<T*,false> : true_type
{ 
};

template<typename T>
struct is_random_access_iterator<const T*,false> : true_type
{ 
};

template<typename T>
struct is_random_access_iterator<volatile T*,false> : true_type
{ 
};

template<typename T>
struct is_random_access_iterator<const volatile T*,false> : true_type
{ 
};

#ifdef __cpp_alias_templates

template<typename T>
constexpr bool is_random_access_iterator_v = is_random_access_iterator<remove_cv_t<T>>::value;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_RANDOM_ACCESS_ITERATOR_HPP
