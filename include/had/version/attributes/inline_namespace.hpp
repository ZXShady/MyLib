#pragma once

#ifndef HAD_VERSION_ATTRIBUTES_INLINE_NAMESPACE_HPP
#define HAD_VERSION_ATTRIBUTES_INLINE_NAMESPACE_HPP 1

#include <had/version/cpp_version.hpp>
#ifdef HAD_CPP11
    #define HAD_INLINE_NAMESPACE inline
#else
    #define HAD_INLINE_NAMESPACE
#endif
#endif // !HAD_VERSION_ATTRIBUTES_INLINE_NAMESPACE_HPP
