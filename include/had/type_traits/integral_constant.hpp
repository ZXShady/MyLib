#pragma once
#ifndef HAD_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP
#define HAD_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP 1

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
HAD_NAMESPACE_BEGIN

template <typename T, T IntValue>
struct integral_constant {
    constexpr static T value = IntValue;

    using this_type  = integral_constant;
    using value_type = T;

    HAD_NODISCARD constexpr inline operator value_type() const noexcept {
        return value;
    }

    HAD_NODISCARD constexpr inline value_type operator()() const noexcept {
        return value;
    }
};


HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP