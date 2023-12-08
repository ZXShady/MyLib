#pragma once

#ifndef HAD_TYPE_TRAITS_ADDRESSOF_HPP
#define HAD_TYPE_TRAITS_ADDRESSOF_HPP 1

#include <had/version/namespace.hpp>
#include <had/typeinfo/operator_addressof.hpp>
#include <had/version/attributes/nodiscard.hpp>
HAD_NAMESPACE_BEGIN
#ifdef __has_builtin
#if __has_builtin(__builtin_addressof)
#define HAD_HAS_BUILTIN_ADDRESSOF
#endif // __has_builtin(__builtin_addressof)
#endif // __has_builtin
#ifdef HAD_HAS_BUILTIN_ADDRESSOF

template<typename T>
HAD_NODISCARD constexpr T* addressof(T& object) noexcept {
    return __builtin_addressof(object);
}

#else // !defined(HAD_HAS_BUILTIN_ADDRESSOF)

template<typename T,typename enable_if<
    operator_addressof<T>::is_overloaded
    >::type = 0 >
HAD_NODISCARD T* addressof(T& object) noexcept {
    return reinterpret_cast<T*>(
        &const_cast<unsigned char&>(
        reinterpret_cast<const volatile unsigned char&>(object)));
}

template<typename T,typename enable_if<
    !operator_addressof<T>::is_overloaded
    >::type = 0 >
constexpr HAD_NODISCARD T* addressof(T& object) noexcept {
    return &object;
}

#endif // HAD_HAS_BUILTIN_ADDRESSOF
template<typename T>
void addressof(const T&&) = delete; // handles const&& and &&

HAD_NAMESPACE_END
#endif // !HAD_TYPE_TRAITS_ADDRESSOF_HPP
