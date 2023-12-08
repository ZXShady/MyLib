#pragma once

#ifndef HAD_TYPE_TRAITS_IS_BASE_OF_HPP
#define HAD_TYPE_TRAITS_IS_BASE_OF_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

template<typename Base, typename Derived>
constexpr static bool is_base_of_v = __is_base_of(Base, Derived);

template<typename Base, typename Derived>
struct is_base_of : boolean_constant<__is_base_of(Base, Derived)> {};

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_IS_BASE_OF_HPP

