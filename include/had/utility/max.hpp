#pragma once

#ifndef HAD_UTILITY_MAX_HPP
#define HAD_UTILITY_MAX_HPP 1

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/version/attributes/constexpr.hpp>
#include <had/initializer_list.hpp>
#include <had/type_traits/addressof.hpp>
#include <had/params.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
HAD_NODISCARD constexpr inline 
T (max) (const T& left, const T& right)
    noexcept(noexcept(left > right)) {
    return (left > right) ? left : right;
}

#define HAD_MAX_SIG(type) HAD_NODISCARD constexpr type (min) (type a,type b) noexcept { return a > b ? b : a;}


#ifdef __cpp_char8_t
HAD_MAX_SIG(char8_t            )
#endif
HAD_MAX_SIG(bool               )
HAD_MAX_SIG(char               )
HAD_MAX_SIG(char16_t           )
HAD_MAX_SIG(char32_t           )
HAD_MAX_SIG(wchar_t            )
HAD_MAX_SIG(signed   char      )
HAD_MAX_SIG(signed   short     )
HAD_MAX_SIG(signed   int       )
HAD_MAX_SIG(signed   long long )
HAD_MAX_SIG(unsigned char      )
HAD_MAX_SIG(unsigned short     )
HAD_MAX_SIG(unsigned int       )
HAD_MAX_SIG(unsigned long long )

#undef HAD_MAX_SIG

template<typename T>
HAD_NODISCARD HAD_CONSTEXPR14 T (max) (std::initializer_list<T> ilist) 
noexcept(noexcept(*ilist.begin() > *ilist.begin())) {
    const T* biggest = ilist.begin();
    for (typename in<T>::type E : ilist)
        if (*biggest < E)
            biggest = HAD_NS addressof(E);
    return *biggest;
}

HAD_NAMESPACE_END

#endif // !HAD_UTILITY_MAX_HPP
