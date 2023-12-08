#pragma once
#include <had/version/cpp_version.hpp>

#ifndef HAD_VERSION_IF_CONSTEXPR_HPP
#define HAD_VERSION_IF_CONSTEXPR_HPP 1

#ifdef HAD_CPP17
    #define HAD_HAS_IF_CONSTEXPR true

    #define HAD_IF_CONSTEXPR_FALLBACK_TO_IF(Condition,Body) \
    if constexpr (Condition) { \
    Body \
    }
    #define HAD_IF_CONSTEXPR(Condition,Body) \
    if constexpr (Condition) { \
    Body \
    }

    #define HAD_ELSE_CONSTEXPR(Body) \
    else { \
    Body \
    }

    #define HAD_ELSE_IF_CONSTEXPR(Condition,Body) \
    else if constexpr (Condition) { \
    Body \
    }
#else
    #define HAD_IF_CONSTEXPR_FALLBACK_TO_IF(Condition,Body) \
    if (Condition) { \
    Body \
    }
    #define HAD_HAS_IF_CONSTEXPR false
    #define HAD_IF_CONSTEXPR(Condition,Body)
    #define HAD_ELSE_CONSTEXPR(Body)
    #define HAD_ELSE_IF_CONSTEXPR(Condition,Body)

#endif

#endif // !HAD_VERSION_IF_CONSTEXPR_HPP
