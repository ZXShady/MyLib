#pragma once
#ifndef HAD_TYPE_TRAITS_AS_HPP
#define HAD_TYPE_TRAITS_AS_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/type_traits/add_reference.hpp>
#include <had/type_traits/add_const.hpp>
#include <had/type_traits/add_volatile.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
HAD_NODISCARD T as(T&& object) noexcept {
    return static_cast<T>(object);
}

template<typename T>
HAD_NODISCARD typename add_lvalue_reference<T>::type as_lvalue(T&& object) noexcept {
    return static_cast<typename add_lvalue_reference<T>::type>(object);
}

template<typename T>
HAD_NODISCARD typename add_rvalue_reference<T>::type as_rvalue(T&& object) noexcept {
    return static_cast<typename add_rvalue_reference<T>::type>(object);
}


template <typename T>
HAD_NODISCARD constexpr const T& as_const(const T& object) noexcept { 
    return object;
}

template <typename T>
void as_const(const volatile T&&) = delete;

template <typename T>
HAD_NODISCARD constexpr volatile T& as_volatile(volatile T& object) noexcept { 
    return object;
}

template <typename T>
void as_volatile(const volatile T&&) = delete;

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_AS_HPP
