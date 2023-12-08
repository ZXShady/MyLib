#pragma once

#ifndef HAD_TYPE_TRAITS_MAKE_INTEGER_SEQUENCE_HPP
#define HAD_TYPE_TRAITS_MAKE_INTEGER_SEQUENCE_HPP

#include <had/type_traits/is_integral.hpp>

HAD_NAMESPACE_BEGIN

template <typename T, T... Values>
struct integer_sequence { // sequence of integer parameters
    static_assert(is_integral_v<T>, "integer_sequence<T, I...> requires T to be an integral type.");

    using value_type = T;

    HAD_NODISCARD static constexpr size_t size() noexcept {
        return sizeof...(Values);
    }
};

template <typename T, T Value>
using make_integer_sequence = __make_integer_seq<integer_sequence, T, Value>;

template <size_t... Ns>
using index_sequence = integer_sequence<size_t, Ns...>;

template <size_t N>
using make_index_sequence = make_integer_sequence<size_t, N>;

template <typename... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_MAKE_INTEGER_SEQUENCE_HPP
