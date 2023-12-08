#pragma once

#ifndef HAD_TYPE_TRAITS_REMOVE_EXTENT_HPP
#define HAD_TYPE_TRAITS_REMOVE_EXTENT_HPP 1
#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>          struct remove_extent        { using type = T; };
template<typename T,size_t N> struct remove_extent<T[N]>  { using type = T; };
template<typename T>          struct remove_extent<T[]>   { using type = T; };

template<typename T>
using remove_extent_t = typename remove_extent<T>::type;

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_REMOVE_EXTENT_HPP
