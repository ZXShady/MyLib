#pragma once

#ifndef HAD_TYPE_TRAITS_MOVE_HPP
#define HAD_TYPE_TRAITS_MOVE_HPP 1

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>

#include <had/type_traits/remove_reference.hpp>
#include <had/config.hpp>

HAD_NAMESPACE_BEGIN


template<typename T>
HAD_NODISCARD constexpr 
inline typename remove_reference<T>::type&& move(T&& obj) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(obj);
}

#ifdef HAD_WARN_IF_MOVING_CONST_OBJECT

template<typename T>
void move(const T&) = delete;

#endif // HAD_WARN_IF_MOVING_CONST_OBJECT

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_MOVE_HPP
