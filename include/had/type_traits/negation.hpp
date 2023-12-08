#pragma once

#ifndef HAD_TYPE_TRAITS_NEGATION_HPP
#define HAD_TYPE_TRAITS_NEGATION_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

template<typename Trait>
struct negation : boolean_constant<!bool(Trait::value)> {};

template<typename Trait>
constexpr bool negation_v = !bool(Trait::value);

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_NEGATION_HPP
