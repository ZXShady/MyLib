#pragma once

#ifndef HAD_TYPE_TRAITS_VOID_T_HPP
#define HAD_TYPE_TRAITS_VOID_T_HPP 1

#include <had/version/namespace.hpp>

HAD_NAMESPACE_BEGIN

template<typename...>
struct void_s {
    using type = void;
};

template <typename...>
using void_t = void;

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_VOID_T_HPP
