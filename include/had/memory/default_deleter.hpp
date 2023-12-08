#pragma once

#ifndef HAD_MEMORY_DEFAULT_DELETER_HPP
#define HAD_MEMORY_DEFAULT_DELETER_HPP

#include <had/macros.hpp>
#include <had/version/namespace.hpp>
#include <had/type_traits/is_convertible.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/version/attributes/constexpr.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
struct default_deleter {
    using value_type = T;
    using pointer_type = value_type*;
    using const_pointer_type = const value_type*;
    using reference_type = value_type&;
    using const_reference_type = const value_type&;

    constexpr default_deleter() noexcept = default;

    template <typename U, typename enable_if<is_convertible<U*, T*>::value >::type = 0>
    constexpr default_deleter(default_deleter<U>) noexcept {}

    HAD_CONSTEXPR20 void operator()(T* ptr) noexcept {
        static_assert(sizeof(T) > 0, "can't delete an incomplete type!");
        delete ptr;
    }
};

template<typename T>
struct default_deleter<T[]> {
    using value_type = T;
    using pointer_type = value_type*;
    using const_pointer_type = const value_type*;
    using reference_type = value_type&;
    using const_reference_type = const value_type&;

     
    constexpr default_deleter() noexcept = default;
    template <typename U, typename enable_if<is_convertible<U (*)[], T (*)[]>::value >::type = 0>
    constexpr default_deleter(default_deleter<U[]>) noexcept {}

    template <typename U, typename enable_if<is_convertible<U (*)[], T(*)[]>::value >::type = 0>
    HAD_CONSTEXPR20 void operator()(U* ptr) const noexcept { 
        static_assert(sizeof(U) > 0, "can't delete an incomplete type");
        delete[] ptr;
    }
};

HAD_NAMESPACE_END

#endif // !HAD_MEMORY_DEFAULT_DELETER_HPP