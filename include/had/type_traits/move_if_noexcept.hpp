#pragma once

#ifndef HAD_TYPE_TRAITS_MOVE_IF_NOEXCEPT_HPP
#define HAD_TYPE_TRAITS_MOVE_IF_NOEXCEPT_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>

#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/is_nothrow_move_constructible.hpp>
#include <had/type_traits/remove_reference.hpp>
#include <had/config.hpp>
#include <had/type_traits/as.hpp>
#include <had/type_traits/conditional.hpp>
#include <had/type_traits/is_copy_constructible.hpp>
HAD_NAMESPACE_BEGIN

template <typename T>
HAD_NODISCARD constexpr
typename conditional<
    !  is_nothrow_move_constructible<T>::value
    && is_copy_constructible<T>::value
    ,const T&
    ,T&&>::type
move_if_noexcept(T& obj) noexcept {
    return HAD_NS as_rvalue(obj);
}

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_MOVE_HPP
