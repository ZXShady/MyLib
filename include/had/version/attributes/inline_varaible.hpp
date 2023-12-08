#pragma once

#ifndef HAD_VERSION_ATTRIBUTES_INLINE_VARIABLE_HPP
#define HAD_VERSION_ATTRIBUTES_INLINE_VARIABLE_HPP 1

#include <had/version/cpp_version.hpp>
#ifdef __cpp_inline_variables
    #define HAD_INLINE_VAR inline
    #define HAD_INLINE_VAR_FALLBACK(fallback) inline
#else
    #define HAD_INLINE_VAR
    #define HAD_INLINE_VAR_FALLBACK(fallback) fallback
#endif // __cpp_inline_variables

#endif // !HAD_VERSION_ATTRIBUTES_INLINE_VARIABLE_HPP
