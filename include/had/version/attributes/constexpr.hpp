#pragma once


#ifndef HAD_VERSION_ATTRIBUTES_CONSTEXPR_HPP
#define HAD_VERSION_ATTRIBUTES_CONSTEXPR_HPP 1

#include <had/version/cpp_version.hpp>
#ifdef HAD_CPP11
    #define HAD_CONSTEXPR11 constexpr
#else
    #define HAD_CONSTEXPR11
#endif

#ifdef HAD_CPP14
    #define HAD_CONSTEXPR14 constexpr
#else
    #define HAD_CONSTEXPR14
#endif

#ifdef HAD_CPP17
    #define HAD_CONSTEXPR17 constexpr
#else
    #define HAD_CONSTEXPR17
#endif

#ifdef HAD_CPP20
    #define HAD_CONSTEXPR20 constexpr
#else
    #define HAD_CONSTEXPR20
#endif

#ifdef HAD_CPP23
    #define HAD_CONSTEXPR23 constexpr
#else
    #define HAD_CONSTEXPR23
#endif

#endif // !HAD_VERSION_ATTRIBUTES_CONSTEXPR_HPP

