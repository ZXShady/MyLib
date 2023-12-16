#ifndef HAD_TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_HPP
#define HAD_TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/is_constructible.hpp>
#include <had/type_traits/is_trivially_constructible.hpp>
#include <had/type_traits/is_nothrow_constructible.hpp>
HAD_NAMESPACE_BEGIN

template<class T>
struct is_default_constructible : is_constructible<T> {};

template<class T>
struct is_trivially_default_constructible : is_trivially_constructible<T> {};

template<class T>
struct is_nothrow_default_constructible : is_nothrow_constructible<T> {};

#ifdef __cpp_variable_templates

template<class T>
constexpr bool is_default_constructible_v = is_constructible_v<T>;

template<class T>
constexpr bool is_trivially_default_constructible_v = is_trivially_constructible_v<T>;

template<class T>
constexpr bool is_nothrow_default_constructible_v = is_nothrow_constructible_v<T>;



#endif // __cpp_variable_templates


HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_HPP
