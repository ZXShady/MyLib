#pragma once

#ifndef HAD_UTILITY_MIN_HPP
#define HAD_UTILITY_MIN_HPP 1

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/initializer_list.hpp>

HAD_NAMESPACE_BEGIN

template<typename T>
constexpr inline HAD_NODISCARD 
auto (min) (const T& a, const T& b)
noexcept(noexcept(a > b))
-> decltype((a < b) ? a : b) {
    return (a < b) ? a : b;
}

#define HAD_MIN_SIG(type) constexpr HAD_NODISCARD type (min) (type a,type b) noexcept { return a > b ? b : a;}


#ifdef __cpp_char8_t
HAD_MIN_SIG(char8_t            )
#endif
HAD_MIN_SIG(bool               )
HAD_MIN_SIG(char               )
HAD_MIN_SIG(char16_t           )
HAD_MIN_SIG(char32_t           )
HAD_MIN_SIG(wchar_t            )
HAD_MIN_SIG(signed   char      )
HAD_MIN_SIG(signed   short     )
HAD_MIN_SIG(signed   int       )
HAD_MIN_SIG(signed   long long )
HAD_MIN_SIG(unsigned char      )
HAD_MIN_SIG(unsigned short     )
HAD_MIN_SIG(unsigned int       )
HAD_MIN_SIG(unsigned long long )

#undef HAD_MIN_SIG

template<typename T1, typename T2,typename Predicate>
constexpr inline HAD_NODISCARD
auto (min) (const T1& a, const T2& b,Predicate predicate) 
noexcept(noexcept(predicate(a,b)))
-> decltype((predicate(a, b)) ? b : a) {
    return (predicate(a,b)) ? b : a;
}

template<typename T>
constexpr inline HAD_NODISCARD
T (min) (initializer_list<T> ilist) 
noexcept(noexcept(*ilist.begin() > *ilist.begin())) {
    const T* smallest = ilist.begin();
    for (const auto& E : ilist)
        if (*smallest < E)
            smallest = &E;
    return *smallest;
}

template<typename T, typename Predicate>
constexpr inline HAD_NODISCARD
T (min)(initializer_list<T> ilist, Predicate predicate)
noexcept(noexcept(predicate(*ilist.begin(), *ilist.begin()))) {
    const T* smallest = ilist.begin();
    for (const auto& E : ilist) {
        smallest = (!predicate(*smallest, E)) ? smallest : &E;
    }
    return *smallest;
}
HAD_NAMESPACE_END

#endif // !HAD_UTILITY_MIN_HPP
