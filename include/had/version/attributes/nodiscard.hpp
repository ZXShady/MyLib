#pragma once


#ifndef HAD_VERSION_ATTRIBUTES_NODISCARD_HPP
#define HAD_VERSION_ATTRIBUTES_NODISCARD_HPP 1

#include <had/version/cpp_version.hpp>

#ifdef HAD_CPP17
#   define HAD_NODISCARD [[nodiscard]]
#else
#   define HAD_NODISCARD
#endif
#ifdef HAD_CPP20
#   define HAD_NODISCARD_MSG(msg) [[nodiscard(msg)]]
#   define HAD_NODISCARD20 [[nodiscard]]
#   define HAD_NODISCARD20_MSG(msg) [[nodiscard(msg)]]
#else 
#   define HAD_NODISCARD_MSG(msg) [[nodiscard]]
#   define HAD_NODISCARD20_MSG(msg) 
#   define HAD_NODISCARD20
#endif

#ifdef HAD_CPP23
#   define HAD_NODISCARD23 [[nodiscard]]
#   define HAD_NODISCARD23_MSG(msg) [[nodiscard(msg)]]

#else 
#   define HAD_NODISCARD23
#   define HAD_NODISCARD23_MSG(msg)
#endif

#endif // !HAD_VERSION_ATTRIBUTES_NODISCARD_HPP

