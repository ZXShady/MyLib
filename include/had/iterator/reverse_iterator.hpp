#pragma once

#ifndef HAD_ITERATOR_REVERSE_ITERATOR_HPP
#define HAD_ITERATOR_REVERSE_ITERATOR_HPP 1

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/utility/declval.hpp>
HAD_NAMESPACE_BEGIN

template<typename ValueType>
class reverse_iterator {
public:
    using pointer_difference_type = ptrdiff_t;
    using this_type = reverse_iterator;
    using value_type = ValueType;
    using pointer_type = value_type*;
    using const_pointer_type = const value_type*;
    using reference_type = value_type&;
    using const_reference_type = const value_type&;

    reverse_iterator() noexcept = default;
    reverse_iterator(pointer_type ptr) noexcept : mPtr(ptr) {}
    HAD_NODISCARD constexpr inline this_type operator+(pointer_difference_type num) const noexcept { return mPtr - num; }
    HAD_NODISCARD constexpr inline this_type operator-(pointer_difference_type num) const noexcept { return mPtr + num; }

    constexpr inline this_type& operator+=(pointer_difference_type num) noexcept { mPtr -= num; return *this; }
    constexpr inline this_type& operator-=(pointer_difference_type num) noexcept { mPtr += num; return *this; }
    constexpr inline this_type operator++(int) noexcept {
        this_type copy(*this);
        ++(*this);
        return copy;
    }
    constexpr inline this_type& operator++() noexcept { 
        --mPtr;
        return *this;
    }
    constexpr inline this_type operator--(int) noexcept {
        this_type copy(*this);
        --(*this);
        return copy;
    }
    constexpr inline this_type& operator--() noexcept { 
        ++mPtr;
        return *this;
    }
    using creft = const this_type&;
    HAD_NODISCARD friend constexpr inline bool operator==(creft a,creft b) noexcept {
        return a.mPtr == b.mPtr;
    }
    HAD_NODISCARD friend constexpr inline bool operator!=(creft a,creft b) noexcept {
        return a.mPtr != b.mPtr;
    }
    HAD_NODISCARD friend constexpr inline bool operator<(creft a, creft b) noexcept {
        return a.mPtr < b.mPtr;
    }
    HAD_NODISCARD friend constexpr inline bool operator>(creft a, creft b) noexcept {
        return a.mPtr > b.mPtr;
    }
    HAD_NODISCARD friend constexpr inline bool operator<=(creft a, creft b) noexcept {
        return a.mPtr <= b.mPtr;
    }
    HAD_NODISCARD friend constexpr inline bool operator>=(creft a, creft b) noexcept {
        return a.mPtr >= b.mPtr;
    }

    HAD_NODISCARD constexpr inline const_pointer_type operator->() const noexcept {
        return mPtr;
    }

    HAD_NODISCARD constexpr inline const_reference_type operator*() const noexcept {
        return *mPtr;
    }

    HAD_NODISCARD constexpr inline pointer_type operator->() noexcept {
        return mPtr;
    }

    HAD_NODISCARD constexpr inline reference_type operator*() noexcept {
        return *mPtr;
    }

    reference_type operator[](size_t index) const noexcept {
        return *(*this + index);
    }

private:
    pointer_type mPtr;
};

HAD_NAMESPACE_END

#endif // !HAD_ITERATOR_REVERSE_ITERATOR_HPP
