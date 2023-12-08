#pragma once

#ifndef HAD_MEMORY_ALIGNED_STORAGE_HPP
#define HAD_MEMORY_ALIGNED_STORAGE_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/biggest_sizeof.hpp>
#include <had/array.hpp>
HAD_NAMESPACE_BEGIN

template<size_t Size,size_t Alignment = alignof(double)>
struct aligned_storage {
    struct type {
        using T = unsigned char;
        using size_type = size_t;
        alignas(Alignment) T _109284901849[Size];
        HAD_NODISCARD constexpr T* data() noexcept { return _109284901849; }
        HAD_NODISCARD constexpr const T* data() const noexcept { return _109284901849; }
        HAD_NODISCARD constexpr volatile T* data() volatile noexcept { return _109284901849; }
        HAD_NODISCARD constexpr const volatile T* data() const volatile noexcept { return _109284901849; }


        HAD_NODISCARD constexpr T& operator[](size_type index) noexcept {
            data()[index];
        } 
        HAD_NODISCARD constexpr const T& operator[](size_type index) const noexcept {
            data()[index];
        }
        HAD_NODISCARD constexpr volatile T& operator[](size_type index) volatile noexcept {
            data()[index];
        }
        HAD_NODISCARD constexpr const volatile T& operator[](size_type index) const volatile noexcept {
            data()[index];
        }

    };
};

#ifdef __cpp_alias_templates

template<size_t Size,size_t Alignment = alignof(double)>
using aligned_storage_t = typename aligned_storage<Size, Alignment>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_MEMORY_ALIGNED_STORAGE_HPP