#pragma once
#ifndef HAD_INITIALIZER_LIST_HPP
#define HAD_INITIALIZER_LIST_HPP 1
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#if __has_include(<initializer_list>)
#include <initializer_list>
#else 
namespace std {
template <typename T>
class initializer_list {
public:
    using value_type = T;
    using reference = const T&;
    using const_reference = const T&;
    using size_type = size_t;

    using iterator = const T*;
    using const_iterator = const T*;

    constexpr initializer_list() noexcept : first(nullptr), last(nullptr) {}

    constexpr initializer_list(const T* first_arg, const T* last_arg) noexcept
        : first(first_arg), last(last_arg) {
    }

    HAD_NODISCARD constexpr const T* begin() const noexcept {
        return first;
    }

    HAD_NODISCARD  constexpr const T* end() const noexcept {
        return last;
    }

    HAD_NODISCARD constexpr size_t size() const noexcept {
        return static_cast<size_t>( last - first );
    }

private:
    const T* first;
    const T* last;
};

template <class T>
HAD_NODISCARD constexpr const T* begin(initializer_list<T> ilist) noexcept {
    return ilist.begin();
}

template <class T>
HAD_NODISCARD constexpr const T* end(initializer_list<T> ilist) noexcept {
    return ilist.end();
}
} // namespace std
#endif // __has_include(<initializer_list>)

HAD_NAMESPACE_BEGIN

template<typename T>
using initializer_list = std::initializer_list<T>;

HAD_NAMESPACE_END

#endif // !HAD_INITIALIZER_LIST_HPP