#pragma once

#ifndef HAD_UTILITY_SWAP_HPP
#define HAD_UTILITY_SWAP_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/is_move_constructible.hpp>
#include <had/type_traits/is_move_assignable.hpp>
#include <had/type_traits/is_nothrow_move_constructible.hpp>
#include <had/type_traits/is_nothrow_move_assignable.hpp>
#include <had/type_traits/move.hpp>
HAD_NAMESPACE_BEGIN

template <typename T>
void swap(T& left, T& right) noexcept(
       is_nothrow_move_constructible<T>::value
    && is_nothrow_move_assignable<T>::value   ) {
    T temp  = HAD_NS move(left);
    left    = HAD_NS move(right);
    right   = HAD_NS move(temp);
}

template <typename T>
void swap(const volatile T&, const volatile T&) = delete;

HAD_NAMESPACE_END

#endif // !HAD_UTILITY_SWAP_HPP
