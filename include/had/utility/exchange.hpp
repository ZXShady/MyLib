#pragma once

#ifndef HAD_UTILITY_EXCHANGE_HPP
#define HAD_UTILITY_EXCHANGE_HPP 1

#include <had/version/namespace.hpp>

HAD_NAMESPACE_BEGIN

template <typename T, typename U = T>
T exchange(T& val, U&& new_val) noexcept {
    T old_val = static_cast<T&&>(val);
    val       = static_cast<U&&>(new_val);
    return old_val;
}

HAD_NAMESPACE_END

#endif // !HAD_UTILITY_EXCHANGE_HPP