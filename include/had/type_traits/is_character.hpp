#pragma once

#ifndef HAD_TYPE_TRAITS_IS_CHARACTER_HPP
#define HAD_TYPE_TRAITS_IS_CHARACTER_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/remove_cv.hpp>
HAD_NAMESPACE_BEGIN

template<typename> struct is_character                : false_type {};
template<>         struct is_character<char>          : true_type  {};
template<>         struct is_character<signed char>   : true_type  {};
template<>         struct is_character<unsigned char> : true_type  {};

#ifdef __cpp_variable_templates

template<typename T>
constexpr bool is_character_v = is_character<T>::value;

#endif // __cpp_alias_templates
HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_CHARACTER_HPP
