#pragma once
#include <had/macros.hpp>
#include <had/memory/new_alignof.hpp>
#include <had/version/namespace.hpp>
#include <had/type_traits/is_function.hpp>
#include <had/type_traits/is_const.hpp>
#include <had/type_traits/is_reference.hpp>
#include <new>

HAD_NAMESPACE_BEGIN

template<typename T>
class allocator {
public:
    static_assert(!is_const<T>::value, "The C++ Standard forbids allocators of const elements "
        "because allocator<const T> is ill-formed.");
    static_assert(!is_function<T>::value, "The C++ Standard forbids allocators for function elements "
        "because of [allocator.requirements].");
    static_assert(!is_reference<T>::value, "The C++ Standard forbids allocators for reference elements "
        "because of [allocator.requirements].");

    using this_type       = allocator;
    using value_type      = T;
    using size_type       = size_t;
    using pointer_type    = T*;
    using difference_type = ptrdiff_t;

    HAD_NODISCARD static T* allocate(size_type count) {
        HAD_STATIC_ASSERT_IF(sizeof(value_type) == 0, "value_type must be complete before calling allocate.");
        return static_cast<pointer_type>(::operator new(sizeof(T) * count));
    }
    
    static void deallocate(T* ptr, size_type count) noexcept {
        ::operator delete(static_cast<void*>(ptr), sizeof(T) * count);
    }

};

template<typename T>
class default_allocator {
public:
    static_assert(!is_const<T>::value, "The C++ Standard forbids allocators of const elements "
        "because allocator<const T> is ill-formed.");
    static_assert(!is_function<T>::value, "The C++ Standard forbids allocators for function elements "
        "because of [allocator.requirements].");
    static_assert(!is_reference<T>::value, "The C++ Standard forbids allocators for reference elements "
        "because of [allocator.requirements].");

    using this_type       = default_allocator;
    using value_type      = T;
    using size_type       = size_t;
    using pointer_type    = T*;
    using difference_type = ptrdiff_t;

    HAD_NODISCARD static T* allocate(size_type count) {
        HAD_STATIC_ASSERT_IF(sizeof(value_type) == 0, "value_type must be complete before calling allocate.");
        return static_cast<pointer_type>(::operator new(sizeof(T) * count));
    }

    static void deallocate(T* ptr, size_type count) noexcept {
        ::operator delete(static_cast<void*>(ptr), sizeof(T) * count);
    }

};

HAD_NAMESPACE_END