#pragma once

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>

#ifndef HAD_CSTRING_H
#define HAD_CSTRING_H

#include <had/stdint.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/version/attributes/constexpr.hpp>
HAD_NAMESPACE_BEGIN

template<typename CharType>
HAD_NODISCARD HAD_CONSTEXPR14 size_t string_length(const CharType* str) noexcept {
    size_t len = 0;
    while (*str++ != CharType('\0'))
        len++;
    return len;
}

template<typename CharType>
HAD_NODISCARD HAD_CONSTEXPR14 size_t string_length_reverse(const CharType* str) noexcept {
    size_t len = 0;
    while (*str-- == CharType('\0'))
        len++;
    return len;
}


HAD_NAMESPACE_END
#endif // !HAD_CSTRING_H
