#pragma once

#include <cstring>

template<typename CharType>
struct char_traits {
    using char_type = CharType;
    using pointer_type = CharType*;
    using const_pointer_type = const CharType*;

    enum special_chars : char_type {
        space = char_type(' '),
        eof = -1
    };

    constexpr static inline void copy(const_pointer_type dst, const_pointer_type src, size_t count) {
        std::memcpy(dst,src,count * sizeof(char_type) );
    }

    constexpr static inline void move(const_pointer_type dst, const_pointer_type src, size_t count) {
        std::memmove(dst,src,count * sizeof(char_type) );
    }
};