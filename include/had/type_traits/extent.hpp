#pragma once

#ifndef HAD_TYPE_TRAITS_EXTENT_HPP
#define HAD_TYPE_TRAITS_EXTENT_HPP 1
#include <had/version/namespace.hpp>
#include <had/type_traits/integral_constant.hpp>
HAD_NAMESPACE_BEGIN

template <class T, size_t Index = 0>
constexpr static size_t extent_v = 0; // determine extent of dimension Index of array T

template <class T, size_t N>
constexpr static size_t extent_v<T[N], 0> = N;

template <class T, size_t Index, size_t N>
constexpr static size_t extent_v<T[N], Index> = extent_v<T, Index - 1>;

template <class T, size_t Index>
constexpr static size_t extent_v<T[], Index> = extent_v<T, Index - 1>;

template <class T, size_t Index = 0>
struct extent : integral_constant < size_t, extent_v<T, Index>> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_EXTENT_HPP
