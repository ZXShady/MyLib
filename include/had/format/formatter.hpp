#pragma once
#ifndef HAD_FORMAT_FORMATTER_HPP
#define HAD_FORMAT_FORMATTER_HPP

#include <had/string.hpp>

HAD_NAMESPACE_BEGIN

template<typename T>
struct formatter {
    HAD_NODISCARD static string to_string(const T& input) {
        return HAD_NS to_string(input);
    }
};


template<>
struct formatter<const char*> {
    HAD_NODISCARD static string to_string(const char* input) {
        return input;
    }
};


template<>
struct formatter<string> {
    HAD_NODISCARD static string to_string(const string& input) {
        return input;
    }
};

/*
template<typename T,size_t N>
struct formatter<T[N]> {
    constexpr static size_t reserve_amount = N * formatter<T>::reserve_amount;
    
    static ::had::string to_string(const T* input) {
        using formatter_t = ::had::formatter<T>;
        if HAD_CONSTEXPR17 (N != 0) {
            ::had::string str = "[ ";
            //str.reserve(reserve_amount);
            for (size_t i = 0; i < N; i++) {
                str += formatter_t::to_string(input[i]);
                str += ", ";
            }
            str.back() = ']';
            *(&str.back() - 1) = ' ';
        return str;
        }
        else {
            return "[]";
        }
    }
};
*/
HAD_NAMESPACE_END

#endif // !HAD_FORMAT_FORMATTER_HPP
