#pragma once

#ifndef HAD_TYPE_TRAITS_FORWARD_HPP
#define HAD_TYPE_TRAITS_FORWARD_HPP 1

#include <had/version/attributes/nodiscard.hpp>
#include <had/version/namespace.hpp>

#include <had/type_traits/remove_reference.hpp>
#include <had/type_traits/is_reference.hpp>
#include <had/type_traits/like.hpp>
HAD_NAMESPACE_BEGIN

template <typename T>
HAD_NODISCARD constexpr T&& forward(remove_reference_t<T>& obj) noexcept {
    return static_cast<T&&>(obj);
}

template <typename T>
HAD_NODISCARD constexpr T&& forward(remove_reference_t<T>&& obj) noexcept {
    return static_cast<T&&>(obj);
}

template<typename T, typename U>
HAD_NODISCARD constexpr typename like<T,U>::type forward_like(U&& object) noexcept {
    using cast = typename like<T, U>::type;
    return static_cast<cast>(object);
}
HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_FORWARD_HPP
