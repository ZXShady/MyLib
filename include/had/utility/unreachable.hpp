#pragma once

#ifndef HAD_UTILITY_UNREACHABLE_HPP
#define HAD_UTILITY_UNREACHABLE_HPP

#include <had/version/namespace.hpp>
HAD_NAMESPACE_BEGIN

[[noreturn]] inline void unreachable() {
    // Uses compiler specific extensions if possible.
    // Even if no extension is used, undefined behavior is still raised by
    // an empty function body and the noreturn attribute.
    // GCC, Clang, ICC
    // MSVC
#if defined(__GNUC__) 
    __builtin_unreachable();
#elif defined(_MSC_VER) 
    __assume(false);
#else 
    throw "unreachable";
#endif


}

HAD_NAMESPACE_END

#endif // !HAD_UTILITY_UNREACHABLE_HPP
