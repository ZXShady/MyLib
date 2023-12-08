#pragma once
#ifndef HAD_PAIR_HPP
#define HAD_PAIR_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>

#include <had/utility/swap.hpp>
#include <had/format/formatter.hpp>
HAD_NAMESPACE_BEGIN

template<typename T,typename U>
struct pair final {
    using first_type  = T;
    using second_type = U;
    T first;
    U second;
    constexpr static bool are_nothrow_swappable = noexcept(swap(first, first)) && noexcept(swap(second, second));

    HAD_CONSTEXPR14 void swap(pair& other) noexcept(are_nothrow_swappable) {
        using had::swap;
        swap(first, other.first);
        swap(second, other.second);
    }

    HAD_CONSTEXPR14 friend void swap(pair& a,pair& b) noexcept(are_nothrow_swappable) {
        a.swap(b);
    }
};


template<typename T,typename U>
struct formatter<pair<T, U>> {
    static string to_string(const pair<T, U>& pair) {
		return format("( first = {}, second = {} )",pair.first,pair.second);
    }
};
HAD_NAMESPACE_END

template<typename T1,typename T2>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator==(const had::pair<T1,T2>& a,const had::pair<T1,T2>& b) noexcept {
    return a.first == b.first && a.second == b.second;
}

template<typename T1,typename T2>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(const had::pair<T1,T2>& a,const had::pair<T1,T2>& b) noexcept {
    return !(a == b);
}

template<typename T1,typename T2>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator<(const had::pair<T1,T2>& a,const had::pair<T1,T2>& b) noexcept {
    return a.first < b.first || a.second < b.second;
}

template<typename T1,typename T2>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator>(const had::pair<T1,T2>& a,const had::pair<T1,T2>& b) noexcept {
    return b > a;
}

template<typename T1,typename T2>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator<=(const had::pair<T1,T2>& a,const had::pair<T1,T2>& b) noexcept {
    return !(a > b);
}

template<typename T1,typename T2>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator>=(const had::pair<T1,T2>& a,const had::pair<T1,T2>& b) noexcept {
    return !(a < b);
}
#endif // !HAD_PAIR_HPP
