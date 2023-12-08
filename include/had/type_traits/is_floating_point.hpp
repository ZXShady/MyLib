#pragma once

#ifndef HAD_TYPE_TRAITS_IS_FLOATING_POINT_HPP
#define HAD_TYPE_TRAITS_IS_FLOATING_POINT_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/is_any_of.hpp>
#include <had/type_traits/remove_cv.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
constexpr bool is_floating_point_v = is_any_of_v<remove_cv_t<T>,
    float,
    double,
    long double
>;

template<typename T>
struct is_floating_point : boolean_constant<is_floating_point_v<T>> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_FLOATING_POINT_HPP
