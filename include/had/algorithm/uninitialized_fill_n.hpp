#pragma once
#ifndef HAD_ALGORITHM_UNITITIALIZED_FILL_N_HPP
#define HAD_ALGORITHM_UNITITIALIZED_FILL_N_HPP 1
#include <had/version/namespace.hpp>
#include <cstring>

HAD_NAMESPACE_BEGIN

template<typename Iterator,typename Value>
constexpr inline void unititialized_fill_n(Iterator begin, size_t count, const Value& value) {
    using T = decltype(*begin);
    while (--count != 0) {
        ::new (begin++) T(value);
    }
    void(count = 0);
}

inline void unititialized_fill_n(char* begin, size_t count, char value) {
    std::memset(begin, value, count);
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_UNITITIALIZED_FILL_N_HPP

